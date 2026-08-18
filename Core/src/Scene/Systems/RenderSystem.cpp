#include "RenderSystem.h"
#include "Renderer/Renderer.h"

namespace NGN
{
	void RenderSystem::OnCreate(Scene& scene)
	{
		NGN_CORE_INFO("RenderSystem Created");
	}

	void RenderSystem::OnUpdate(Scene& scene, Timestep ts)
	{
		NGN_PROFILE_FUNCTION();
		UpdateCameraFrustum(scene);
		UpdateVisibleEntities(scene);
	}

	void RenderSystem::UpdateCameraFrustum(Scene& scene)
	{
		Entity camEntity = scene.GetPrimaryCamera();
		if (!camEntity.IsValid())
			NGN_CORE_ASSERT(false, "No primary camera found in scene");

		auto& camComp = camEntity.GetComponent<CameraComponent>();
		auto& camTransform = camEntity.GetComponent<TransformComponent>();

		camComp.Camera.RecalculateViewMatrix(camTransform.Translation, camTransform.Rotation);
		Renderer::SetCamera(camComp.Camera);
		m_Frustum = Renderer::GetSceneData().frustum;
	}

	void RenderSystem::UpdateVisibleEntities(Scene& scene)
	{
		NGN_PROFILE_FUNCTION();
		entt::registry& registry = scene.GetRegistry();

		if (m_BVHDirty)
		{
			m_BVH.Build(registry);
			m_BVHDirty = false;
		}
		else
		{
			m_BVH.UpdateDynamicBvhScene(registry);
		}

		m_VisibleEntities.clear();
		m_BVH.QueryFrustum(m_Frustum, m_VisibleEntities);

		for (entt::entity handle : m_VisibleEntities)
		{
			if (!registry.has<MeshComponent, TransformComponent>(handle))
				continue;

			Renderer::Submit(BuildRenderItem(Entity(handle, &scene)));
		}
	}

	RenderItem RenderSystem::BuildRenderItem(Entity entity)
	{
		const auto& transform = entity.GetComponent<TransformComponent>();
		const auto& mesh = entity.GetComponent<MeshComponent>();
		glm::mat4 model = transform.GetTransform();

		RenderItem item;
		item.modelMatrix = model;
		item.inverseModelMatrix = glm::inverse(model);
		item.aabbMin = glm::vec4(mesh.boundsMin, 1.0f);
		item.aabbMax = glm::vec4(mesh.boundsMax, 1.0f);
		item.mesh = mesh.MeshRef.get();
		item.tintColorR = mesh.Color.r;
		item.tintColorG = mesh.Color.g;
		item.tintColorB = mesh.Color.b;
		item.objectType = static_cast<int32_t>(ObjectType::STATIC_MESH);

		return item;
	}
}
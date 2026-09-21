#include "RenderSystem.h"
#include "Render/FrameManager.h"

namespace NGN
{
	void RenderSystem::OnCreate(Scene& scene)
	{
		
	}
	void RenderSystem::OnUpdate(Scene& scene, Timestep ts)
	{
		NGN_PROFILE_FUNCTION();

		auto& registry = scene.GetRegistry();
		// TODO: Change this later for multi-viewport support - "GetActiveCameras()"
		//		a member linking a scenecamera to viewport - then build out ViewportDrawCommands foreach
		auto& camComp = scene.GetPrimaryCamera().GetComponent<CameraComponent>();
		Renderer::FrameManager::SubmitViewportData(camComp.Camera);

		const Frustum& frustum = camComp.ViewFrustum;

		SceneBVH& bvh = scene.GetBVH();
		if (!bvh.IsBuilt()) bvh.Build(registry);
		else bvh.UpdateDynamicBvhScene(registry);

		glm::vec3 fMin = frustum.GetBoundsMin();
		glm::vec3 fMax = frustum.GetBoundsMax();
		/*NGN_CORE_INFO("Frustum AABB: min [{},{},{}] max [{},{},{}]", fMin.x, fMin.y, fMin.z, fMax.x, fMax.y, fMax.z);*/

		std::vector<entt::entity> visibleEntities;
		bvh.QueryFrustum(frustum, visibleEntities);
		/*NGN_CORE_INFO("BVH returned '{}' visible entities", visibleEntities.size());*/

		// Static Meshes
		for (auto entity : visibleEntities)
		{
			auto& transform = registry.get<TransformComponent>(entity);
			auto& mesh = registry.get<MeshComponent>(entity);
			auto& id = registry.get<IDComponent>(entity);

			glm::mat4 entityTransform = transform.GetTransformMatrix();

			for (const SubMesh& subMesh : mesh.SubMeshes) {
				RenderItem renderItem;
				renderItem.objectId = id.ID;
				renderItem.meshId = subMesh.meshId;
				renderItem.blendMode = subMesh.blendMode;
				renderItem.boundsMin = mesh.boundsMin;
				renderItem.boundsMax = mesh.boundsMax;
				renderItem.gpuData.transform = entityTransform * subMesh.localTransform;
				renderItem.gpuData.color = mesh.Color;

				Renderer::FrameManager::SubmitRenderItem(renderItem);
			}
		}

		Renderer::FrameManager::UpdateDrawCommandsSet();
	}
}
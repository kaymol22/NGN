#include "RenderSystem.h"
#include "Render/FrameManager.h"

namespace NGN
{
	void RenderSystem::OnCreate(Scene& scene)
	{
		
	}
	void RenderSystem::OnUpdate(Scene& scene, Timestep ts)
	{

		auto& camComp = scene.GetPrimaryCamera().GetComponent<CameraComponent>();
		Renderer::FrameManager::UpdateViewportData(camComp.Camera);

		

		auto view = scene.GetRegistry().view<TransformComponent, MeshComponent, IDComponent>();
		for (auto entity : view)
		{
			auto [transform, mesh] = view.get<TransformComponent, MeshComponent>(entity);
			auto id = view.get<IDComponent>(entity);
			RenderObject renderObject;
			mesh.UpdateBounds(transform);
			renderObject.objectId = id.ID;
			renderObject.transform = transform.GetTransformMatrix();
			renderObject.color = mesh.Color;
			renderObject.boundsMin = mesh.boundsMin;
			renderObject.boundsMax = mesh.boundsMax;

			Renderer::FrameManager::SubmitRenderObject(renderObject);
		}
	}
}
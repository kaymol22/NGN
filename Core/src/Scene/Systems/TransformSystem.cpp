#include "TransformSystem.h"

namespace NGN
{
	void TransformSystem::OnCreate(Scene& scene) {
		
	}

	void TransformSystem::OnUpdate(Scene& scene, Timestep ts)
	{
		auto& registry = scene.GetRegistry();
		SceneBVH& bvh = scene.GetBVH();

		auto view = registry.view<TransformComponent, IDComponent>();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			if (!transform.IsDirty) continue;

			transform.UpdateMatrix();

			if (auto* mesh = registry.try_get<MeshComponent>(entity)) {
				mesh->UpdateBounds(transform);
			}

			auto& id = view.get<IDComponent>(entity);
			bvh.MarkDirty(id.ID);

			if (auto* cam = registry.try_get<CameraComponent>(entity)) {
				cam->ViewDirty = true;
			}
		}
	}
}
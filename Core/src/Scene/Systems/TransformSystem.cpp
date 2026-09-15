#include "TransformSystem.h"

namespace NGN
{
	void TransformSystem::OnCreate(Scene& scene) {
		
	}

	void TransformSystem::OnUpdate(Scene& scene, Timestep ts)
	{
		auto view = scene.GetRegistry().view<TransformComponent>();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			transform.UpdateMatrix();
		}
	}
}
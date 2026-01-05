#pragma once

#include "System.h"
#include "Scene.h"
#include "Components.h"

namespace NGN
{
	class TransformDebugSystem : public System
	{
	public:
		void OnUpdate(Scene& scene, float dt) override
		{
			auto& world = scene.GetWorld();
			auto& transforms = world.GetAll<TransformComponent>();

			for (size_t i = 0; i < transforms.size(); i++)
			{
				const auto& t = transforms[i];
				std::cout << "Entity " << i << " Position: "
					<< t.Translation.x << ", "
					<< t.Translation.y << ", "
					<< t.Translation.z << "\n";
			}
		}
	};
}
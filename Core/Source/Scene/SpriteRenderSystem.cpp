#include "SpriteRenderSystem.h"
#include "Renderer/Renderer.h"

#include <iostream>

namespace NGN
{
	void SpriteRenderSystem::OnRender(Scene& scene) 
	{
		auto& world = scene.GetWorld();

		auto& transforms = world.GetAll<TransformComponent>();
		auto& sprites = world.GetAll<SpriteRendererComponent>();

		for (size_t i = 0; i < sprites.size(); i++)
		{
			std::cout << "Entity with sprite" << std::endl;
		}
	}
}
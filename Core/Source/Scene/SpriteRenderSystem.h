#pragma once

#include "Scene.h"
#include "System.h"
#include "Components.h"

namespace NGN
{
	class SpriteRenderSystem : public System
	{
	public:
		void OnRender(Scene& scene) override
		{
			auto& world = scene.GetWorld();

			auto& transforms = world.GetAll<TransformComponent>();
			auto& sprites = world.GetAll<SpriteRendererComponent>();

			/*Renderer::BeginScene(m_ViewProjection)*/
		}
	};
}
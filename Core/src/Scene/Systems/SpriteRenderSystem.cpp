#include "SpriteRenderSystem.h"

#include "SpriteRenderSystem.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer.h"

#include <iostream>

namespace NGN
{
	void SpriteRenderSystem::OnCreate(Scene& scene)
	{
		NGN_PROFILE_FUNCTION();
	}

	void SpriteRenderSystem::OnRender(Scene& scene, const SceneCamera& camera)
	{
		NGN_PROFILE_FUNCTION();

		auto& registry = scene.GetRegistry();

		auto view = registry.view<TransformComponent, SpriteRendererComponent>();

		for (auto entity : view)
		{
			auto& transform = registry.get<TransformComponent>(entity);
			auto sprite = registry.get<SpriteRendererComponent>(entity);
			auto id = registry.get<IDComponent>(entity);

			if (sprite.SubTexture)
			{
				Renderer::Submit2D(
					transform.GetTransform(),
					sprite.SubTexture,
					sprite.TilingFactor,
					sprite.Color,
					id.ID
				);
			}
			else if (sprite.Texture)
			{
				Renderer::Submit2D(
					transform.GetTransform(),
					sprite.Texture,
					sprite.TilingFactor,
					sprite.Color,
					id.ID
				);
			}
			else
			{
				Renderer::Submit2D(
					transform.GetTransform(),
					sprite.Color,
					id.ID
				);
			}
		}
	}

	void SpriteRenderSystem::OnUpdate(Scene& scene, Timestep ts)
	{
		NGN_PROFILE_FUNCTION();
	}
}

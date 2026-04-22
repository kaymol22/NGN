#include "SpriteRenderSystem.h"

#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer2D.h"

#include <iostream>

//TEMP: Debugging
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/string_cast.hpp>

namespace NGN
{
	void SpriteRenderSystem::OnCreate(Scene& scene)
	{
		NGN_PROFILE_FUNCTION();
	}

	// TODO: Should implement EventListener for when new entity added to scene - 
	// call to cache entities with SpriteRendererComponent for faster iteration (avoid registry view each frame)

	// void OnEntityAdded(Event& event) {}

	void SpriteRenderSystem::OnRender(Scene& scene, const SceneCamera& camera)
	{
		NGN_PROFILE_FUNCTION();

		auto& registry = scene.GetRegistry();

		Renderer2D::BeginScene(camera);
		
		auto view = registry.view<TransformComponent, SpriteRendererComponent>();

		for (auto entity : view)
		{
			auto& transform = registry.get<TransformComponent>(entity);
			auto sprite = registry.get<SpriteRendererComponent>(entity);
			auto id = registry.get<IDComponent>(entity);

			if (sprite.SubTexture)
			{
				Renderer2D::DrawQuad(
					transform.Translation,
					transform.Scale,
					sprite.SubTexture,
					sprite.TilingFactor,
					sprite.Color,
					id.ID
				);
			}

			else if (sprite.Texture)
			{
				Renderer2D::DrawQuad(
					transform.Translation,
					transform.Scale,
					sprite.Texture,
					sprite.TilingFactor,
					sprite.Color,
					id.ID
				);
			}
		}
		Renderer2D::EndScene();
	}

	void SpriteRenderSystem::OnUpdate(Scene& scene, Timestep ts)
	{
		NGN_PROFILE_FUNCTION();
	}
}
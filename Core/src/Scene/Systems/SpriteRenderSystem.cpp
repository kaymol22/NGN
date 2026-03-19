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

	}

	// TODO: Should implement EventListener for when new entity added to scene - 
	// call to cache entities with SpriteRendererComponent for faster iteration (avoid registry view each frame)

	// void OnEntityAdded(Event& event) {}

	void SpriteRenderSystem::OnRender(Scene& scene)
	{
		NGN_PROFILE_FUNCTION();

		// Use over GetEntitiesWithComponents to avoid creating temporary vector of entities
		auto& registry = scene.GetRegistry();

		Renderer2D::BeginScene(scene.GetViewProjection());

		auto view = registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : view)
		{
			auto& id = registry.get<IDComponent>(entity);
			auto& transform = registry.get<TransformComponent>(entity);
			auto& sprite = registry.get<SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, id.ID);
		}
		Renderer2D::EndScene();
	}
}
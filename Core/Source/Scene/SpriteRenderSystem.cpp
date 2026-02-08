#include "SpriteRenderSystem.h"

#include "Components.h"
#include "Scene.h"
#include "Renderer/Renderer2D.h"

#include <iostream>

//TEMP: Debugging
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace NGN
{
	void SpriteRenderSystem::OnCreate(Scene& scene)
	{

	}

	void SpriteRenderSystem::OnRender(Scene& scene)
	{
		World& world = scene.GetWorld();

		/*Renderer2D::BeginScene();

		Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), { 100,100,0 }), {1,0,0,1}, 0);

		for (Entity entity : world.View<TransformComponent, SpriteRendererComponent>())
		{
			auto& transform = world.GetComponent<TransformComponent>(entity);
			auto& sprite = world.GetComponent<SpriteRendererComponent>(entity);

			NGN_CORE_INFO("Rendering Entity {0}: Translation={1}, Scale={2}",
				entity.ID,
				glm::to_string(transform.Translation),
				glm::to_string(transform.Scale));

			Renderer2D::DrawQuad(
				transform.GetTransform(),
				sprite.Color,
				entity.ID
			);
		}

		Renderer2D::EndScene();*/
	}
}
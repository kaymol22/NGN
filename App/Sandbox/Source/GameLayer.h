#pragma once

#include "Core/Layer.h"
#include "Core/Log.h"

#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/TransformDebugSystem.h"
#include "Scene/SpriteRenderSystem.h"

#include "Renderer/Renderer2D.h"

#include <iostream>
//TEMP: debug
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class GameLayer : public NGN::Layer
{
public:
	GameLayer()
		: Layer("Game", NGN::LayerFlags::WorldSpace)
		, m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{}
	
	void OnAttach() override
	{
		NGN::Log::GetCoreLogger()->info("GameLayer attached");

		m_Scene = std::make_unique<NGN::Scene>();

		auto a = m_Scene->CreateEntity();
		auto b = m_Scene->CreateEntity();

		auto& aTransform = m_Scene->AddComponent<NGN::TransformComponent>(a);
		aTransform.Translation = { 0, 0, 0 };
		aTransform.Scale = { 100, 100, 1 };
		
		auto& bTransform = m_Scene->AddComponent<NGN::TransformComponent>(b);
		bTransform.Translation = { -100, 100, 0 };
		bTransform.Scale = { 50, 50, 1 };

		auto& aColor = m_Scene->AddComponent<NGN::SpriteRendererComponent>(a);
		auto& bColor = m_Scene->AddComponent<NGN::SpriteRendererComponent>(b);

		aColor.Color = { 1.0f, 0.1f, 0.1f, 1.0f };
		bColor.Color = { 0.1f, 1.0f, 0.1f, 1.0f };

		m_Scene->AddSystem<NGN::SpriteRenderSystem>();
		/*m_Scene->AddSystem<NGN::TransformDebugSystem>();*/

		m_Camera.SetProjection(-1.0f, 1.0f, -1.0f, 1.0f);
		m_Camera.SetPosition({ 0.0f, 0.0f, 0.0f });
	}

	void OnUpdate(NGN::Timestep ts)
	{
		m_Scene->OnUpdate(ts);
	}

	void OnRender()
	{
		NGN_CORE_INFO("GameLayer::OnRender()");
		m_Scene->OnRender();

		NGN_INFO("VP= {0}", glm::to_string(m_Camera.GetViewProjectionMatrix()));
		NGN::Renderer2D::BeginScene(m_Camera.GetViewProjectionMatrix());
		NGN::Renderer2D::DrawQuad(
			glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)),
			{ 1, 0, 0, 1 },
			0
		);
		NGN::Renderer2D::EndScene();
	}
	
private:
	std::unique_ptr<NGN::Scene> m_Scene;
	NGN::OrthographicCamera m_Camera;
};
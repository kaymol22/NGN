#pragma once

#include "NGN.h"

#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Systems/SpriteRenderSystem.h"

#include "Renderer/Renderer2D.h"

#include <iostream>
//TEMP: debug
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class GameLayer : public NGN::Layer
{
public:
	GameLayer()
		: Layer("Game", NGN::LayerFlags::WorldSpace), m_CameraController(1280.0f / 720.0f)
	{}
	
	void OnAttach() override
	{
		NGN_PROFILE_FUNCTION();

		m_Scene = std::make_unique<NGN::Scene>();

		auto a = m_Scene->CreateEntity("Entity A");
		auto b = m_Scene->CreateEntity("Entity B");

		auto& aTransform = a.GetComponent<NGN::TransformComponent>();
		aTransform.Translation = { 0.5f, -0.5f, 0.0f };
		aTransform.Scale = { 0.25f, 0.25f, 1.0f };
		
		auto& bTransform = b.GetComponent<NGN::TransformComponent>();
		bTransform.Translation = { 0.5f, 0.5f, 0.0f };
		bTransform.Scale = { 0.25f, 0.25f, 1.0f };

		a.AddComponent<NGN::SpriteRendererComponent>(glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f });
		b.AddComponent<NGN::SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		m_Scene->AddSystem<NGN::SpriteRenderSystem>();


	}

	void OnUpdate(NGN::Timestep ts)
	{
		m_CameraController.OnUpdate(ts);
		m_Scene->OnUpdate(ts);
	}

	void OnRender()
	{
		NGN_PROFILE_FUNCTION();

		m_Scene->SetActiveCamera(m_CameraController.GetCamera().GetViewProjectionMatrix());
		m_Scene->OnRender();

		/*NGN_INFO("VP= {0}", glm::to_string(m_Camera.GetViewProjectionMatrix()));*/
	}

	void OnEvent(NGN::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}
	
private:
	std::unique_ptr<NGN::Scene> m_Scene;
	NGN::OrthographicCameraController m_CameraController;
};
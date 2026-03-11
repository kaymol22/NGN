#pragma once

#include "Core/Layer.h"
#include "Core/Log.h"
#include "Core/Input.h"
#include "Events/KeyEvent.h"

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
		: Layer("Game", NGN::LayerFlags::WorldSpace)
		, m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
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

		m_Camera.SetProjection(-1.0f, 1.0f, -1.0f, 1.0f);
		m_Camera.SetPosition({ 0.0f, 0.0f, 0.0f });
	}

	void OnUpdate(NGN::Timestep ts)
	{
		if (NGN::Input::IsKeyPressed(NGN::Key::A))
		{
			NGN_INFO("Key A is pressed");
		}

		m_Scene->OnUpdate(ts);
	}

	void OnRender()
	{
		NGN_PROFILE_FUNCTION();

		m_Scene->SetActiveCamera(m_Camera.GetProjectionMatrix());
		m_Scene->OnRender();

		/*NGN_INFO("VP= {0}", glm::to_string(m_Camera.GetViewProjectionMatrix()));*/
	}

	void OnEvent(NGN::Event& e) override
	{
		NGN::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<NGN::KeyPressedEvent>
			([this](NGN::KeyPressedEvent& e)
			{
				NGN_INFO("Event: Key {0} pressed", e.GetKeyCode());
				return false;
			}
		);
	}
	
private:
	std::unique_ptr<NGN::Scene> m_Scene;
	NGN::OrthographicCamera m_Camera;
};
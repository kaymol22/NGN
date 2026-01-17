#pragma once

#include "Core/Layer.h"
#include "Core/Log.h"

#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/TransformDebugSystem.h"

#include <iostream>

class TestLayer : public NGN::Layer
{
public:
	TestLayer() = default;
	
	void OnAttach() override
	{
		NGN::Log::GetCoreLogger()->info("TestLayer attached");

		m_Scene = std::make_unique<NGN::Scene>();

		auto a = m_Scene->CreateEntity();
		auto b = m_Scene->CreateEntity();

		m_Scene->AddComponent<NGN::TransformComponent>(a).Translation = { 1,2,3 };
		m_Scene->AddComponent<NGN::TransformComponent>(b).Translation = { 4,5,6 };

		m_Scene->AddComponent<NGN::SpriteRendererComponent>(a);
		m_Scene->AddComponent<NGN::SpriteRendererComponent>(b);

		m_Scene->AddSystem<NGN::TransformDebugSystem>();
	}

	void OnUpdate(float ts)
	{
		m_Scene->OnUpdate(ts);
	}
	
private:
	std::unique_ptr<NGN::Scene> m_Scene;
};
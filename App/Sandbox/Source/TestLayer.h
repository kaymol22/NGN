#pragma once

#include "Core/Layer.h"
#include "Core/Log.h"

#include "Scene/Scene.h"
#include "Scene/Components.h"

#include <iostream>

class TestLayer : public NGN::Layer
{
public:
	TestLayer() = default;
	
	void OnAttach() override
	{
		NGN::Log::GetCoreLogger()->info("TestLayer attached");

		// Create entities
		m_EntityA = m_Scene.CreateEntity();
		m_EntityB = m_Scene.CreateEntity();

		// Add TransformComponent
		auto& tA = m_Scene.AddComponent<NGN::TransformComponent>(m_EntityA);
		tA.Translation = { 1.0f, 2.0f, 3.0f };

		auto& tB = m_Scene.AddComponent<NGN::TransformComponent>(m_EntityB);
		tB.Translation = { 4.0f, 5.0f, 6.0f };

		// Add SpriteRenderer Component
		m_Scene.AddComponent<NGN::SpriteRendererComponent>(m_EntityA).Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		m_Scene.AddComponent<NGN::SpriteRendererComponent>(m_EntityB).Color = { 0.0f, 1.0f, 0.0f, 1.0f };
	}

	void OnUpdate(float ts)
	{
		auto& transforms = m_Scene.GetAll<NGN::TransformComponent>();
		for (size_t i = 0; i < transforms.size(); i++)
		{
			const auto& t = transforms[i];
			std::cout << "Entity" << i << "Position: "
				<< t.Translation.x << ", "
				<< t.Translation.y << ", "
				<< t.Translation.z << "\n";
		}
	}
	
private:
	NGN::Scene m_Scene;
	NGN::Entity m_EntityA;
	NGN::Entity m_EntityB;
};
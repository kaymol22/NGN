#include "Core/ngnpch.h"
#include "eScene.h"
#include "Components.h"

namespace NGN
{
	eScene::eScene()
	{
		entt::entity entity1 = m_Registry.create();
		m_Registry.emplace<SpriteRendererComponent>(entity1);
	}

	eScene::~eScene()
	{

	}

	void eScene::OnUpdate(Timestep ts)
	{

	}
}
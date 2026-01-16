#include "Scene.h"

namespace NGN
{
	Scene::Scene() = default;

	Entity Scene::CreateEntity()
	{
		return m_World.CreateEntity();
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_World.DestroyEntity(entity);
	}

	void Scene::OnUpdate(float dt)
	{
		for (auto& system : m_Systems)
			system->OnUpdate(*this, dt);
	}

	void Scene::OnRender()
	{
		for (auto& system : m_Systems)
			system->OnRender(*this);
	}
}
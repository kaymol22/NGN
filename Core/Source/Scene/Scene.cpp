#include "Scene.h"

namespace NGN
{
	Scene::Scene() = default;

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
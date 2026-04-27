#include "ngnpch.h"
#include "SceneManager.h"

namespace NGN
{
	// Create scene on client side - transfer to scene manager using std::move
	void SceneManager::RegisterScene(const std::string& name, Scope<Scene> scene)
	{
		NGN_CORE_ASSERT(!HasScene(name), "Scene '{}' already registered", name);
		NGN_CORE_ASSERT(scene, "Cannot register null scene");

		m_Scenes[name] = std::move(scene);
		NGN_CORE_INFO("Scene '{}' registered", name);
	}

	Scene* SceneManager::GetScene(const std::string& name)
	{
		auto target = m_Scenes.find(name);
		if (target != m_Scenes.end())
			return target->second.get();
		NGN_CORE_WARN("Scene '{}' not found", name);
		return nullptr;
	}

	void SceneManager::SetActiveScene(const std::string& name)
	{
		if (!HasScene(name))
		{
			NGN_CORE_ERROR("Cannot set active scene '{}' - not registered", name);
			return;
		}

		m_ActiveScene = m_Scenes[name].get();
		m_ActiveSceneName = name;
		NGN_CORE_INFO("Active scene set to '{}'", name);
	}

	void SceneManager::UnregisterScene(const std::string& name)
	{
		if (!HasScene(name))
		{
			NGN_CORE_WARN("Cannot unregister scene '{}' - not found", name);
			return;
		}

		if (m_ActiveSceneName == name)
			m_ActiveScene = nullptr;

		m_Scenes.erase(name);
		NGN_CORE_INFO("Scene '{}' unregistered", name);
	}

	bool SceneManager::HasScene(const std::string& name) const
	{
		return m_Scenes.find(name) != m_Scenes.end();
	}
}
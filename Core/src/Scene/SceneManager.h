#pragma once
#include "Scene.h"

namespace NGN
{
	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void RegisterScene(const std::string& name, Scope<Scene> scene);

		Scene* GetScene(const std::string& name);
		Scene* GetActiveScene() { return m_ActiveScene; }

		void SetActiveScene(const std::string& name);
		void UnregisterScene(const std::string& name);

		bool HasScene(const std::string& name) const;
	
	private:
		std::unordered_map<std::string, Scope<Scene>> m_Scenes;
		Scene* m_ActiveScene = nullptr;
		std::string m_ActiveSceneName;
	};
}
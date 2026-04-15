#pragma once
#include "Scene.h"

namespace NGN
{
	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void RegisterScene(const std::string& name, Ref<Scene> scene);

		Ref<Scene> GetScene(const std::string& name);
		Ref<Scene> GetActiveScene() { return m_ActiveScene; }
		
		void SetActiveScene(const std::string& name);
		void UnregisterScene(const std::string& name);

		bool HasScene(const std::string& name) const;
	
	private:
		std::unordered_map<std::string, Ref<Scene>> m_Scenes;
		Ref<Scene> m_ActiveScene;
		std::string m_ActiveSceneName;
	};
}
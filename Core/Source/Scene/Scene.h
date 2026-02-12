#pragma once

#include "World.h"
#include "System.h"
#include "Entity.h"

namespace NGN
{
	class Scene 
	{
	public:
		Scene();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		void OnUpdate(float dt);
		void OnRender();

		World& GetWorld() { return m_World; }
		// To be used to fetch systems
		const World& GetWorld() const { return m_World; }

		// ============ Components ============ //
		template<typename T, typename... Args>
		T& AddComponent(Entity entity, Args&&... args)
		{
			return m_World.AddComponent<T>(entity);
		}

		template<typename T>
		bool HasComponent(Entity entity)
		{
			return m_World.HasComponent<T>(entity);
		}

		template<typename T>
		bool GetComponent(Entity entity)
		{
			return m_World.GetComponent<T>(entity);
		}
		// ============ Systems ============ //
		template<typename TSystem, typename... Args>
		TSystem& AddSystem(Args&&... args)
		{
			static_assert(std::is_base_of_v<System, TSystem>,
				"TSystem must derive from System");

			auto system = std::make_unique<TSystem>(std::forward<Args>(args)...);
			TSystem& ref = *system;

			system->OnCreate(*this);
			m_Systems.push_back(std::move(system));

			return ref;
		}
		// ============ Camera ============ //
		void SetActiveCamera(const glm::mat4& viewProj)
		{
			m_ViewProjection = viewProj;
		}

		const glm::mat4& GetViewProjection() const
		{
			return m_ViewProjection;
		}

	private:
		World m_World;
		std::vector<std::unique_ptr<System>> m_Systems;
		glm::mat4 m_ViewProjection;
	};
}
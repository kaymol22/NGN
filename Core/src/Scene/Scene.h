#pragma once

#include <entt.hpp>
#include "ngnpch.h"

#include "Core/Timestep.h"
#include "Core/UUID.h"
#include "Components.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace NGN
{
	class System;
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name);
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name);
		void DestroyEntity(Entity entity);

		Entity GetEntityByUUID(UUID id);
		bool EntityExists(UUID id) const;

		void OnUpdate(Timestep ts);
		void OnRender();

		// Return all entities with specific components - variadic for multiple 
		template <typename... Components>
		auto GetEntitiesWithComponents()
		{
			static_assert(sizeof... (Components) > 0, "Must specify at least one component type");
			auto view = m_Registry.view<Components...>();
			std::vector<Entity> results;
			for (auto entity : view)
			{
				results.emplace_back(entity, this);
			}
			return results;
		}

		// For registry queries + component management
		entt::registry& GetRegistry() { return m_Registry; }
		const entt::registry& GetRegistry() const { return m_Registry; }

		// ================== Systems ================== //
		template <typename TSystem, typename... Args>
		TSystem& AddSystem(Args&&... args)
		{
			/*NGN_CORE_ASSERT(std::is_base_of_v<System, TSystem>, "TSystem must be derived from System");*/
			
			auto system = std::make_unique<TSystem>(std::forward<Args>(args)...);
			TSystem& ref = *system;

			ref.OnCreate(*this);
			m_Systems.push_back(std::move(system));

			return ref;
		}

		// ============ Camera ============ //
		void SetActiveCamera(const glm::mat4& viewProj) { m_ViewProjection = viewProj; }

		const glm::mat4& GetViewProjection() const
		{
			return m_ViewProjection;
		}

	private:
		entt::registry m_Registry;
		std::vector<std::unique_ptr<System>> m_Systems;
		std::unordered_map<UUID, entt::entity> m_EntityMap;
		glm::mat4 m_ViewProjection;

		friend class Entity;
	};
}

#include "Entity.h"
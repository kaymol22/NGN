#pragma once
#include <entt.hpp>

#include "Scene.h"
#include "Components.h"

namespace NGN
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);

		template<typename T>
		bool HasComponent()
		{
			NGN_CORE_ASSERT(m_Scene != nullptr, "Entity does not belong to a scene");
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			NGN_CORE_ASSERT(m_Scene != nullptr, "Entity does not belong to a scene");
			NGN_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			NGN_CORE_ASSERT(m_Scene != nullptr, "Entity does not belong to a scene");
			NGN_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			NGN_CORE_ASSERT(m_Scene != nullptr, "Entity does not belong to a scene");
			NGN_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		entt::entity GetHandle() const { return m_EntityHandle; }
		bool IsValid() const { return m_EntityHandle != entt::null; }

		UUID GetUUID()
		{
			NGN_CORE_ASSERT(m_Scene != nullptr, "Entity does not belong to a scene");
			return GetComponent<IDComponent>().ID;
		}

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		explicit operator bool() const { return IsValid(); }
		explicit operator entt::entity() const { return m_EntityHandle; }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;

		friend class Scene;
	};
}
#pragma once

#include "Entity.h"
#include "ComponentPoolBase.h"
#include "ComponentPool.h"

#include "Core/ngnpch.h"
#include <typeindex>

namespace NGN
{
	class Scene
	{
	public:

		Entity CreateEntity()
		{
			return Entity{ m_NextEntityID++ };
		}

		void DestroyEntity(Entity entity)
		{
			for (auto& [_, pool] : m_ComponentPools)
				pool->Remove(entity.ID);
		}

		template<typename T>
		T& AddComponent(Entity entity)
		{
			return GetOrCreatePool<T>.Add(entity.ID);
		}

		template<typename T>
		bool HasComponent(Entity entity)
		{
			auto comp = m_ComponentPools.find(typeid(T));
			if (comp == m_ComponentPools.end())
				return false;
			return static_cast<ComponentPool<T>*>(comp->second.get())->Has(entity.ID);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return GetPool<T>().Get(entity.ID);
		}

		template<typename T>
		std::vector<T>& GetAll()
		{
			return GetPool<T>().Data();
		}

	private:
		template<typename T>
		ComponentPool<T>& GetPool()
		{
			return *static_cast<ComponentPool<T>*>(m_ComponentPools.at(typeid(T)).get());
		}

		template<typename T>
		ComponentPool<T>& GetOrCreatePool()
		{
			auto type = typeid(T);
			if (!m_ComponentPools.contains(type))
				m_ComponentPools[type] = std::make_unique<ComponentPool<T>>();
			return *static_cast<ComponentPool<T>*>(m_ComponentPools[type].get());
		}

	private:
		EntityID m_NextEntityID;
		std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_ComponentPools;
	};
}
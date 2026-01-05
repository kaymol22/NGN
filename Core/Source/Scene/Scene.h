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

		Scene() : m_NextEntityID(1) {}

		Entity CreateEntity()
		{
			return Entity{ m_NextEntityID++ };
		}

		void DestroyEntity(Entity entity)
		{
			for (auto& [_, pool] : m_ComponentPools)
				if (pool->Has(entity.ID))
					pool->Remove(entity.ID);
		}

		template<typename T>
		T& AddComponent(Entity entity)
		{
			auto& comp = GetOrCreatePool<T>().Add(entity.ID);
			NGN::Log::GetCoreLogger()->info("Added component pool: {0} for Entity {1}", typeid(T).name(), entity.ID);
			return comp;

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
			auto it = m_ComponentPools.find(typeid(T));
			assert(it != m_ComponentPools.end() && "Component pool does not exist");
			return static_cast<ComponentPool<T>*>(it->second.get())->Data();
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
			const auto& type = typeid(T);
			if (!m_ComponentPools.contains(type))
				m_ComponentPools[type] = std::make_unique<ComponentPool<T>>();
			return *static_cast<ComponentPool<T>*>(m_ComponentPools[type].get());
		}

	private:
		EntityID m_NextEntityID;
		std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_ComponentPools;
	};
}
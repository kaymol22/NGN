#pragma once 

#include "Core/ngnpch.h"
#include <cstdint>

namespace NGN
{
	using EntityID = uint32_t;

	template<typename T>
	class ComponentPool : public IComponentPool
	{
	public:
		T& Add(EntityID entity)
		{
			assert(!Has(entity) && "Entity already has component");

			size_t index = m_Data.size();
			m_Data.emplace_back();

			// Update lookup tables
			m_EntityToIndex[entity] = index;
			m_IndexToEntity[index] = entity;

			return m_Data.back();
		}

		bool Has(EntityID entity) const
		{
			return m_EntityToIndex.find(entity) != m_EntityToIndex.end();
		}

		// Zero indirection lookup method*
		T& Get(EntityID entity)
		{
			assert(Has(entity) && "Entity does not have component");
			return m_Data[m_EntityToIndex[entity]];
		}

		// Removal method that avoids shifting elements/broken indices - CANNOT RELY ON COMPONENT ORDER
		void Remove(EntityID entity)
		{
			assert(Has(entity) && "Entity does not have component");

			size_t index = m_EntityToIndex[entity];
			size_t last = m_Data.size() - 1;

			// Move target to last slot in array
			m_Data[index] = std::move(m_Data[last]);

			EntityID movedEntity = m_IndexToEntity[last];
			m_EntityToIndex[movedEntity] = index;
			m_IndexToEntity[index] = movedEntity;

			m_Data.pop_back();
			m_EntityToIndex.erase(entity);
			m_IndexToEntity.erase(last);
		}

		std::vector<T>& Data() { return m_Data; }
		const std::vector<T>& Data() const { return m_Data; }

	private:
		std::vector<T> m_Data;
		std::unordered_map<EntityID, size_t> m_EntityToIndex;
		std::unordered_map<size_t, EntityID> m_IndexToEntity;
	};
}
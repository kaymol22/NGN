#pragma once

#include "Entity.h"
#include <unordered_map>
#include <memory>
#include <cstdint>

namespace NGN
{
	class Scene
	{
	public:
		Scene() : m_NextID(1) {}

		Entity& CreateEntity()
		{
			uint32_t id = m_NextID++;
			auto entity = std::make_unique<Entity>(id);
			Entity& ref = *entity;
			m_Entities[id] = std::move(entity);
			return ref;
		}

		Entity* GetEntity(uint32_t id)
		{
			auto ent = m_Entities.find(id);
			if (ent != m_Entities.end())
				return ent->second.get();
			return nullptr;
		}

		template<typename T>
		std::vector<Entity*> GetAllWithComponent()
		{
			std::vector<Entity*> result;
			for (auto& [id, entity] : m_Entities)
				if (entity->HasComponent<T>())
					result.push_back(entity.get());
			return result;
		}

	private:
		uint32_t m_NextID;
		std::unordered_map<uint32_t, std::unique_ptr<Entity>> m_Entities;
	};
}
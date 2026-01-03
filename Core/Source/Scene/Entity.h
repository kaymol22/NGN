#pragma once

//#include "Components/TransformComponent.h"
#include <memory>
#include <unordered_map>
#include <typeindex>

#include "Components/Component.h"

namespace NGN
{
	// NOTE: Only one component per type with this design - type acts as unique identifier for loookup**
	class Entity
	{
	public:
		Entity(uint32_t id) : m_ID(id) {}
		~Entity() = default;

		// Add component to entity - template arg as multiple component 'types'
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			std::unique_ptr<T> component = std::make_unique<T>(std::forward<Args>(args));
			T& ref = *component;
			m_Components[typeid(T)] = std::move(component);
			return ref;
		}

		// Get component if exists
		template<typename T>
		T* GetComponent()
		{
			auto comp = m_Components.find(typeid(T));
			if (comp != m_Components.end())
				return static_cast<T*>(comp->second.get());
			return nullptr;
		}

		// Check if entity has specified component
		template<typename T>
		bool HasComponent() const
		{
			return m_Components.find(typeid(T)) != m_Components.end();
		}

		uint32_t GetID() const;

	private:
		uint32_t m_ID;
		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;
	};
}
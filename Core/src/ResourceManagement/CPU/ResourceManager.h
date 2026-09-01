#pragma once

#include "Types/Resource.h"
#include "Types/Texture.h"
#include "ResourceHandle.h"

#include <unordered_map>
#include <memory>
#include <typeindex>

namespace RS
{
	class ResourceManager
	{
	public:
		void UnloadAll();

		template<typename T>
		ResourceHandle<T> Load(const std::string& resourceId)
		{
			static_assert(std::is_base_of_v<Resource, T>, "T must derive from Resource");

			// a. Check resource cache to avoid redundant loading
			auto& typeResources = m_Resources[std::type_index(typeid(T))];
			auto it = typeResources.find(resourceId);

			if (it != typeResources.end())
			{
				// Exists in cache - increment ref count and return handle
				it->second.refCount++;
				return ResourceHandle<T>(resourceId, this);
			}

			// b. Create new resource instance and attempt loading
			auto resource = std::make_shared<T>(resourceId);
			if (!resource->Load())
			{
				// Failed load - return invalid handle to avoid cache corruption
				return ResourceHandle<T>();
			}

			// c. Cache successful resource and track ref
			typeResources[resourceId] = { resource, 1 };
			
			NGN_CORE_INFO("Resource Handle returned for resource with ID: {}", resourceId);
			return ResourceHandle<T>(resourceId, this);
		}

		template<typename T>
		T* GetResource(const std::string& resourceId)
		{
			auto typeIt = m_Resources.find(std::type_index(typeid(T)));
			if (typeIt == m_Resources.end()) return nullptr;

			auto it = typeIt->second.find(resourceId);
			if (it != typeIt->second.end())
				return static_cast<T*>(it->second.resource.get());

			return nullptr;
		}

		// Ref count increment for already-cached resource
		template <typename T>
		void AddRef(const std::string& resourceId)
		{
			auto typeIt = m_Resources.find(std::type_index(typeid(T)));
			if (typeIt == m_Resources.end()) return;

			auto it = typeIt->second.find(resourceId);
			if (it == typeIt->second.end()) return;

			it->second.refCount++;
		}

		// Decrements ref count of resource type T, removes from cache when reaches zero
		template <typename T>
		void Release(const std::string& resourceId)
		{
			auto typeIt = m_Resources.find(std::type_index(typeid(T)));
			if (typeIt == m_Resources.end()) return;

			auto it = typeIt->second.find(resourceId);
			if (it == typeIt->second.end()) return;

			it->second.refCount--;
			if (it.refCount <= 0)
			{
				it->second.resource->Unload();
				typeIt->second.erase(it);
			}
		}

		template<typename T>
		bool HasResource(const std::string& resourceId)
		{
			auto typeIt = m_Resources.find(std::type_index(typeid(T)));
			if (typeIt == m_Resources.end()) return false;
			return typeIt->second.find(resourceId) != typeIt->second.end();
		}
		
		ResourceHandle<RS::Texture> GetDefaultTexture() {
			return Load<RS::Texture>("Checkerboard");
		}

	private:
		struct ResourceData {
			std::shared_ptr<Resource> resource;
			int refCount = 0;
		};

		std::unordered_map <std::type_index,
			std::unordered_map<std::string, ResourceData>> m_Resources;
	};
}
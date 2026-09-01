#pragma once
#include "ResourceManager.h"

namespace RS
{
	class ResourceManager;

	template<typename T>
	class ResourceHandle
	{
	public:
		ResourceHandle() : resourceManager(nullptr) {}
		// TODO: Implement handle resetting properly
		~ResourceHandle() = default;

		ResourceHandle(const std::string& id, ResourceManager* manager) :
			resourceId(id), resourceManager(manager) {
		}

		// Copy - shared ownership but increment refcount
		ResourceHandle(const ResourceHandle& other) :
			resourceId(other.resourceId), resourceManager(other.resourceManager)
		{
			if (resourceManager && !resourceId.empty())
				resourceManager->AddRef(resourceId);
		}

		/*ResourceHandle& operator=(ResourceHandle&& other) noexcept
		{
			if (this == &other) return *this;
			Reset();

			resourceId = std::move(other.resourceId);
			resourceManager = other.resourceManager;

			other.resourceManager = nullptr;
			other.resourceId.clear();

			return *this;
		}*/

		/*~ResourceHandle()
		{
			Reset();
		}*/

		T* Get() const {
			if (!resourceManager) return nullptr;
			return resourceManager->GetResource(resourceId);
		}

		bool IsValid() const {
			return resourceManager && resourceManager->HasResource(resourceId);
		}

		const std::string& GetId() const {
			return resourceId;
		}

		// Convenience Operators
		T* operator->() const {
			return Get();
		}

		T& operator*() const {
			return *Get();
		}

		operator bool() const {
			return IsValid();
		}
	private:
		// Releases currently held resources ref
		/*void Reset()
		{
			if (resourceManager && !resourceId.empty())
				resourceManager->Release(resourceId);

			resourceManager = nullptr;
			resourceId.clear();
		}*/
	private:
		std::string resourceId;
		ResourceManager* resourceManager;
	};
}
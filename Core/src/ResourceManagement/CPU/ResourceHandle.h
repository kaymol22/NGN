#pragma once

#include <string>

namespace RS
{
	class ResourceManager;

	template<typename T>
	class ResourceHandle
	{
	public:
		ResourceHandle() : resourceManager(nullptr) {}

		ResourceHandle(const std::string& id, ResourceManager* manager) :
			resourceId(id), resourceManager(manager) {
		}

		// Copy - shared ownership but increment refcount
		ResourceHandle(const ResourceHandle& other) :
			resourceId(other.resourceId), resourceManager(other.resourceManager)
		{
			if (resourceManager && !resourceId.empty())
				resourceManager->template AddRef<T>(resourceId);
		}

		// Copy assignment - release current ref then share others
		ResourceHandle& operator=(const ResourceHandle& other)
		{
			if (this == &other) return *this;
			
			if (other.resourceManager && !other.resourceId.empty())
				other.resourceManager->template AddRef<T>(other.resourceId);

			Reset();

			resourceId = other.resourceId;
			resourceManager = other.resourceManager;

			return *this;
		}

		ResourceHandle(ResourceHandle&& other) noexcept :
			resourceId(std::move(other.resourceId)), resourceManager(other.resourceManager)
		{
			other.resourceManager = nullptr;
			other.resourceId.clear();
		}

		ResourceHandle& operator=(ResourceHandle&& other) noexcept
		{
			if (this == &other) return *this;
			Reset();

			resourceId = std::move(other.resourceId);
			resourceManager = other.resourceManager;

			other.resourceManager = nullptr;
			other.resourceId.clear();

			return *this;
		}

		~ResourceHandle()
		{
			Reset();
		}

		T* Get() const {
			if (!resourceManager) return nullptr;
			return resourceManager->template GetResource<T>(resourceId);
		}

		bool IsValid() const {
			return resourceManager && resourceManager->template HasResource<T>(resourceId);
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
		void Reset()
		{
			if (resourceManager && !resourceId.empty())
				resourceManager->template Release<T>(resourceId);

			resourceManager = nullptr;
			resourceId.clear();
		}
	private:
		std::string resourceId;
		ResourceManager* resourceManager;
	};
}
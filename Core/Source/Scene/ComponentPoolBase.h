#pragma once
#include <cstdint>

namespace NGN
{
	using EntityID = uint32_t;

	class IComponentPool
	{
	public:
		virtual ~IComponentPool() = default;
		virtual void Remove(EntityID entity) = 0;
	};
}
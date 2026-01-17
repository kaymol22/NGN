#pragma once

#include <cstdint>

namespace NGN
{
	using EntityID = uint32_t;

	struct Entity
	{
		EntityID ID = 0;

		constexpr bool operator==(const Entity& other) const
		{
			return ID == other.ID;
		}

		constexpr bool operator!=(const Entity& other) const
		{
			return ID != other.ID;
		}

		explicit operator bool() const { return ID != 0; }
	};
}
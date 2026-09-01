#pragma once

#include <cstdint>
#include <string>

namespace NGN
{
	// FNV-1a hash function - better for multiple platforms and less collisions than std::hash*
	inline uint64_t HasResourceId(const std::string& id)
	{
		constexpr uint64_t kOffset = 14695981039346656037ull;
		constexpr uint64_t kPrime = 1099511628211ull;

		uint64_t hash = kOffset;
		for (unsigned char c : id)
		{
			hash ^= static_cast<uint64_t>(c);
			hash *= kPrime;
		}
	}
}
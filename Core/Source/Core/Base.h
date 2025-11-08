#pragma once

#include "PlatformDetection.h"


#if defined(NGN_PLATFORM_WINDOWS)
	#define NGN_DEBUGBREAK() __debugbreak()
#elif defined(_linux__)
	#define NGN_DEBUGBREAK() raise(SIGTRAP)
#else
	#define NGN_DEBUGBREAK()
#endif

#ifdef NGN_DEBUG
	#define NGN_ENABLE_ASSERTS
#endif

// For complex macro building
#define NGN_EXPAND_MACRO(x) x
#define NGN_STRINGIFY_MACRO(x) #x

// Bit flag helper
#define BIT(x) (1 << x)

// Function binding helper (event callbacks)
// Creates callable capturing 'this' that forwards args to member function
#define NGN_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { \
	return this->fn(std::forward<decltype(args)>(args)...); \
}

// Smart pointer aliases
namespace NGN {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args){
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

//#include "Core/Log.h"
//#include "Core/Assert.h"
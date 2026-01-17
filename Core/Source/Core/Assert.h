#pragma once

#include "Log.h"
#include "ngnpch.h"

#ifdef NGN_ENABLE_ASSERTS

	#define NGN_CORE_ASSERT(x, ...)									\
	{																\
		if (!(x))													\
		{															\
			NGN_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);	\
			NGN_DEBUGBREAK();										\
		}															\
	}

	#define NGN_ASSERT(x, ...)										\
	{																\
		if (!(x))													\
		{															\
			NGN_ERROR("Assertion Failed: {0}", __VA_ARGS__);		\
			NGN_DEBUGBREAK();										\
		}															\
	}		


#else
	#define NGN_CORE_ASSERT(x, ...)
	#define NGN_ASSERT(x, ...)

#endif
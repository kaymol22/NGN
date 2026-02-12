#pragma once
#include "Instrumentor.h"

#define NGN_PROFILE 1

#if NGN_PROFILE

	#if defined(__GNUC__) || defined(__clang__)
		#define NGN_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define NGN_FUNC_SIG __FUNCSIG__
	#else
		#define NGN_FUNC_SIG "NGN_FUNC_SIG"
	#endif

	#define NGN_PROFILE_BEGIN_SESSION(name, filePath) ::NGN::Instrumentor::Get().BeginSession(name, filePath)
	#define NGN_PROFILE_END_SESSION() ::NGN::Instrumentor::Get().EndSession()

	#define NGN_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = NGN::InstrumentorUtils::CleanOutputString(name, "__cdecl "); \
												::NGN::InstrumentationTimer timer##line(fixedName##line.Data)

	
	#define NGN_PROFILE_SCOPE_LINE(name, line) \
		NGN_PROFILE_SCOPE_LINE2(name, line)

	#define NGN_PROFILE_SCOPE(name) \
		NGN_PROFILE_SCOPE_LINE(name, __LINE__)

	#define NGN_PROFILE_FUNCTION() \
		NGN_PROFILE_SCOPE(NGN_FUNC_SIG)

#else

	#define NGN_PROFILE_BEGIN_SESSION(name, filePath)
	#define NGN_PROFILE_END_SESSION()
	#define NGN_PROFILE_SCOPE(name)
	#define NGN_PROFILE_FUNCTION()

#endif
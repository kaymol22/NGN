#pragma once

#include "Base.h"

#include <spdlog/spdlog.h>

namespace NGN {

	class Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core Log macros
#define NGN_CORE_FATAL(...) ::NGN::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define NGN_CORE_ERROR(...) ::NGN::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NGN_CORE_WARN(...)  ::NGN::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NGN_CORE_INFO(...)  ::NGN::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NGN_CORE_TRACE(...) ::NGN::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client Log macros
#define NGN_FATAL(...) ::NGN::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define NGN_ERROR(...) ::NGN::Log::GetClientLogger()->error(__VA_ARGS__)
#define NGN_WARN(...)  ::NGN::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NGN_INFO(...)  ::NGN::Log::GetClientLogger()->info(__VA_ARGS__)
#define NGN_TRACE(...) ::NGN::Log::GetClientLogger()->trace(__VA_ARGS__)

//TODO: Strip out Macros in Dist build
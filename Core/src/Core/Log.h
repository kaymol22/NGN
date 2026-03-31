#pragma once

#include "Base.h"

#include <spdlog/spdlog.h>

namespace NGN {

	class Log {
	public:
		// Singleton creation on first call - safe destruction at app exit
		static Log& Get();

		std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
		std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

		Log(const Log&) = delete;
		Log(Log&&) = delete;
		Log& operator=(const Log&) = delete;
		Log& operator=(Log&&) = delete;

	private:
		Log();
		~Log() = default;

		std::shared_ptr<spdlog::logger> m_CoreLogger;
		std::shared_ptr<spdlog::logger> m_ClientLogger;
	};
}

#define NGN_CORE_FATAL(...) ::NGN::Log::Get().GetCoreLogger()->fatal(__VA_ARGS__)
#define NGN_CORE_ERROR(...) ::NGN::Log::Get().GetCoreLogger()->error(__VA_ARGS__)
#define NGN_CORE_WARN(...)  ::NGN::Log::Get().GetCoreLogger()->warn(__VA_ARGS__)
#define NGN_CORE_INFO(...)  ::NGN::Log::Get().GetCoreLogger()->info(__VA_ARGS__)
#define NGN_CORE_TRACE(...) ::NGN::Log::Get().GetCoreLogger()->trace(__VA_ARGS__)

#define NGN_FATAL(...) ::NGN::Log::Get().GetClientLogger()->fatal(__VA_ARGS__)
#define NGN_ERROR(...) ::NGN::Log::Get().GetClientLogger()->error(__VA_ARGS__)
#define NGN_WARN(...)  ::NGN::Log::Get().GetClientLogger()->warn(__VA_ARGS__)
#define NGN_INFO(...)  ::NGN::Log::Get().GetClientLogger()->info(__VA_ARGS__)
#define NGN_TRACE(...) ::NGN::Log::Get().GetClientLogger()->trace(__VA_ARGS__)

//TODO: Strip out Macros in Dist build
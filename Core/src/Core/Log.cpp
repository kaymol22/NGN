#include "ngnpch.h"

#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace NGN
{
	Log& Log::Get()
	{
		static Log instance;  // Created once on first call, destroyed at program exit
		return instance;
	}

	Log::Log()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		m_CoreLogger = spdlog::stdout_color_mt("NGN");
		m_CoreLogger->set_level(spdlog::level::trace);
		
		m_ClientLogger = spdlog::stdout_color_mt("APP");
		m_ClientLogger->set_level(spdlog::level::trace);
	}
}
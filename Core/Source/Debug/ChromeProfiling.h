#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>

namespace NGN::Profiling
{
	inline void OpenChromeTracing(const std::string& filePath)
	{
		std::filesystem::path absPath = std::filesystem::absolute(filePath);

		#if defined(_WIN32)
			std::string command = "start \"\" chrome \"" + absPath.string() + "\"";
		#elif defined(__APPLE__)
			std::string command = "open -a \"Google Chrome\" \"chrome://tracing\" --args --load-tracing=\""
				+ absPath.string() + "\"";
		#else
			std::string command = "google-chrome --new-window \"chrome://tracing\" --args --load-tracing=\""
				+ absPath.string() + "\"";
		#endif

			int result = std::system(command.c_str());
			if (result != 0)
				NGN_CORE_WARN("Failed to launch Chrome for Tracing. Result = {}", result);
	}
}
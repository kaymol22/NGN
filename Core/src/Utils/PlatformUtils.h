#pragma once

#include "Core/Timestep.h"

#include <string>

namespace NGN
{
	class FileDialogs
	{
	public: 
		static std::string OpenFile(const char* path);
		static std::string SaveFile(const char* path);
	};

	class Time 
	{
	public:
		static float GetTime();
	};
}
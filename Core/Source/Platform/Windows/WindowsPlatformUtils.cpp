#pragma once

#include "Utils/PlatformUtils.h"

#include <GLFW/glfw3.h>

namespace NGN
{
	float Time::GetTime()
	{
		return glfwGetTime();
	}
}
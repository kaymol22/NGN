#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "OpenGLContext.h"
#include "Core/ngnpch.h"

namespace NGN
{
	OpenGLContext::OpenGLContext(void* windowHandle) : m_WindowHandle(windowHandle)
	{
		NGN_CORE_ASSERT(windowHandle, "Window handle is null, could not create OpenGL context");
	}

	void OpenGLContext::Init()
	{
		NGN_PROFILE_FUNCTION();

		auto* glfwWindow = static_cast<GLFWwindow*>(m_WindowHandle);

		glfwMakeContextCurrent(glfwWindow);

		if (!gladLoadGL(glfwGetProcAddress))
		{
			NGN_CORE_ASSERT(false, "Failed to initialize GLAD");
		}
	}

	void OpenGLContext::SwapBuffers()
	{
		NGN_PROFILE_FUNCTION();

		glfwSwapBuffers(static_cast<GLFWwindow*>(m_WindowHandle));
	}
}
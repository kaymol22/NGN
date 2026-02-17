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

		NGN_CORE_INFO("OpenGL Info:");
		NGN_CORE_INFO("	Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		NGN_CORE_INFO("	Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		NGN_CORE_INFO("	Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		NGN_PROFILE_FUNCTION();

		glfwSwapBuffers(static_cast<GLFWwindow*>(m_WindowHandle));
	}
}
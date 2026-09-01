#include "Core/PlatformDetection.h"
#include "GL_Backend.h"
#include "GL_Utils.h"
#include "GL_Commands.h"
#include "Types/GL_Texture.h"

#include <GLFW/glfw3.h>

namespace OpenGL
{
	void OpenGLBackend::Init(void* nativeWindowHandle)
	{
		auto m_WindowHandle = static_cast<GLFWwindow*>(nativeWindowHandle);
		glfwMakeContextCurrent(m_WindowHandle);
		if (!gladLoadGL(glfwGetProcAddress))
		{
			NGN_CORE_ASSERT(false, "Failed to initialize GLAD");
		}

#ifdef NGN_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		OpenGL::Utils::InitOpenGLDebugMessageCallback();
#endif
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE); // Easy when glfw handles FB - otherwise MSAA FBO setup needed
		glEnable(GL_CULL_FACE);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLBackend::BeginFrame()
	{
		NGN_CORE_INFO("OpenGLBackend BeginFrame");
	}

	void OpenGLBackend::EndFrame()
	{
		NGN_CORE_INFO("OpenGLBackend EndFrame");
	}

	void OpenGLBackend::Shutdown()
	{
		NGN_CORE_INFO("OpenGLBackend shutdown");
	}

	void OpenGLBackend::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{

	}

	void AllocateTextureMemory(RS::Texture& texture)
	{
		uint64_t glId = texture.GetGLId();
		/*OpenGLTexture& texture = OpenGL::OpenGLResourceManager::GetGLTexture(glId);*/
	}

	void OpenGLBackend::UpdateBindlessTextures()
	{
		uint32_t highestBindlessIndex = -1;


	}
}
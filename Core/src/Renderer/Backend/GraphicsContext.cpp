#include "GraphicsContext.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/GL_Context.h"

namespace NGN
{
	Scope<GraphicsContextOld> GraphicsContextOld::CreateGraphicsContext(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
			case API::UNDEFINED:				NGN_CORE_ASSERT(false, "RendererAPI::None not supported"); return nullptr;
			case API::OPENGL:			return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
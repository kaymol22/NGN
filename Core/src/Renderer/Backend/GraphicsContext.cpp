#include "GraphicsContext.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace NGN
{
	Scope<GraphicsContext> GraphicsContext::CreateGraphicsContext(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPIType::UNDEFINED:				NGN_CORE_ASSERT(false, "RendererAPI::None not supported"); return nullptr;
			case RendererAPIType::OPENGL:			return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
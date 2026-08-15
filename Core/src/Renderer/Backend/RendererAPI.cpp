#include "RendererAPI.h"

#include "Platform/OpenGL/GL_RendererAPI.h"

namespace NGN
{ 	
	RendererAPIType RendererAPI::s_API = RendererAPIType::OPENGL;
	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPIType::UNDEFINED:
			NGN_CORE_ASSERT(false, "RendererAPI::UNDEFINED not supported");
			return nullptr;

		case RendererAPIType::OPENGL:
			return CreateScope<OpenGLRendererAPI>();

		case RendererAPIType::VULKAN:
			NGN_CORE_ERROR("Vulkan API not supported yet");
			return nullptr;
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
#include "RendererAPI.h"

#include "Platform/OpenGL/GL_RendererAPI.h"

namespace NGN
{ 	
	API RendererAPI::s_API = API::OPENGL;
	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case API::UNDEFINED:
			NGN_CORE_ASSERT(false, "RendererAPI::UNDEFINED not supported");
			return nullptr;

		case API::OPENGL:
			return CreateScope<OpenGLRendererAPI>();

		case API::VULKAN:
			NGN_CORE_ERROR("Vulkan API not supported yet");
			return nullptr;
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
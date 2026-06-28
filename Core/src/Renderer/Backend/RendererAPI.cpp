#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

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
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
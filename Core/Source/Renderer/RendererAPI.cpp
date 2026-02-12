#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace NGN
{ 	
	RendererAPIType RendererAPI::s_API = RendererAPIType::OpenGL;
	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPIType::None:
			NGN_CORE_ASSERT(false, "RendererAPI::None not supported");
			return nullptr;

		case RendererAPIType::OpenGL:
			return CreateScope<OpenGLRendererAPI>();
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
#include "Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

// TODO: Abstract to interact with Renderer instead
#include "Renderer/Backend/RendererAPI.h"

namespace NGN
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPIType::UNDEFINED:				NGN_CORE_ASSERT(false, "RendererAPI::None not supported"); return nullptr;
			case RendererAPIType::OPENGL:			return CreateRef<OpenGLFramebuffer>(specification);
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
#include "VertexArray.h"
#include "Platform/OpenGL/Resources/GL_VertexArray.h"
#include "Renderer/Renderer.h"

namespace NGN
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case API::UNDEFINED:
				NGN_CORE_ASSERT(false, "RendererAPIType::None not supported");
				return nullptr;
			case API::OPENGL:
				return CreateRef<OpenGLVertexArray>();
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPIType");
		return nullptr;
	}
}
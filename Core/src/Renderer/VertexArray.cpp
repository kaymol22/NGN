#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

namespace NGN
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPIType::None:
				NGN_CORE_ASSERT(false, "RendererAPIType::None not supported");
				return nullptr;
			case RendererAPIType::OpenGL:
				return CreateRef<OpenGLVertexArray>();
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPIType");
		return nullptr;
	}
}
#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace NGN
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPIType::None:
				NGN_CORE_ASSERT(false, "RendererAPI::None not supported");
				return nullptr;
			case RendererAPIType::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(size);
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPIType");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:
			NGN_CORE_ASSERT(false, "RendererAPI::None not supported");
			return nullptr;
		case RendererAPIType::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPIType");
		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:
			NGN_CORE_ASSERT(false, "RendererAPI::None not supported");
			return nullptr;
		case RendererAPIType::OpenGL:
			return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPIType");
		return nullptr;
	}
}
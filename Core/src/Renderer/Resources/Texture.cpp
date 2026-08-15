#include "Texture.h"
#include "Platform/OpenGL/Resources/GL_Texture.h"
//#include "GLUtils.h"

// TODO: Abstract to interact with Renderer instead
#include "Renderer/Backend/RendererAPI.h"

namespace NGN
{
	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPIType::UNDEFINED:		NGN_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
			case RendererAPIType::OPENGL:	return CreateRef<OpenGLTexture2D>(specification);
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPIType::UNDEFINED:		NGN_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
			case RendererAPIType::OPENGL:	return CreateRef<OpenGLTexture2D>(filepath);
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
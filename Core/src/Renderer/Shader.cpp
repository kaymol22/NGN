#include "ngnpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace NGN
{
	Ref<Shader> Shader::Create(
		const std::string& name,
		const std::filesystem::path& vertexSrc,
		const std::filesystem::path& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPIType::None:
				NGN_CORE_ASSERT(false, "Failed to create graphics shader using RendererAPIType::None");
				return nullptr;
			case RendererAPIType::OpenGL:
				return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPIType");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::filesystem::path& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:
			NGN_CORE_ASSERT(false, "Failed to create compute shader using RendererAPIType::None");
			return nullptr;
		case RendererAPIType::OpenGL:
			return CreateRef<OpenGLShader>(filePath);
		}

		NGN_CORE_ASSERT(false, "Unknown RendererAPIType");
		return nullptr;
	}
}
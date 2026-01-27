#pragma once

namespace Renderer {

	uint32_t CreateComputeShader(const std::filesystem::path& path);
	uint32_t ReloadComputeShader(uint32_t shaderHandle, const std::filesystem::path& path);

	uint32_t CreateGraphicsShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	uint32_t ReloadGraphicsShader(uint32_t shaderHandle, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

}

namespace NGN
{
	enum class ShaderStage
	{
		Vertex,
		Fragment,
		Compute
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Reload() = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::filesystem::path& filePath);

		static Ref<Shader> Create(
			const std::string& name,
			const std::filesystem::path& vertexSrc,
			const std::filesystem::path& fragmentSrc
		);
	};

	// Implement library for loading built in shaders
}
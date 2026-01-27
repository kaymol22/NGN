#pragma once

#include "Renderer/Shader.h"

#include <glad/gl.h>

namespace NGN
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::filesystem::path& filePath);
		OpenGLShader(const std::string& name,
			const std::filesystem::path& vertexSrc,
			const std::filesystem::path& fragmentSrc);

		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		void Reload() override;

		virtual const std::string& GetName() const override { return m_Name; }

	private:
		static std::string ReadTextFile(const std::filesystem::path& filePath);
		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
		void CompileAndLink(const std::unordered_map<GLenum, std::string>& sources);

	private:
		uint32_t m_RendererID = 0;
		std::string m_Name;
		// Sources for disk layer reloading
		std::unordered_map<GLenum, std::filesystem::path> m_SourcePaths;
	};
}
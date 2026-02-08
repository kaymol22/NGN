#pragma once

#include "Renderer/Shader.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

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

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual const std::string& GetName() const override { return m_Name; }

	private:
		static std::string ReadTextFile(const std::filesystem::path& filePath);
		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
		void CompileAndLink(const std::unordered_map<GLenum, std::string>& sources);
		int GetUniformLocation(const std::string& name);

	private:
		uint32_t m_RendererID = 0;
		std::string m_Name;
		// Sources for disk layer reloading
		std::unordered_map<GLenum, std::filesystem::path> m_SourcePaths;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}
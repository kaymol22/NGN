#pragma once
#include <glm/glm.hpp>
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>

namespace OpenGL
{
	struct OpenGLShaderModule
	{
		OpenGLShaderModule(const std::string& fileName, const std::vector<std::string>& defines);
		uint32_t GetHandle();
		bool CompilationFailed();
		std::string& GetFileName();
		std::string& GetErrors();
		std::vector<std::string>& GetLineMap();
		const std::string& GetFinalSource() const { return m_FinalSource; }

	private:
		uint32_t m_Handle = 0;
		std::string m_FileName = "";
		std::string m_Errors = "";
		std::vector<std::string> m_LineMap;
		std::string m_FinalSource = "";
	};

	struct OpenGLShader
	{
		OpenGLShader() = default;
		OpenGLShader(std::vector<std::string> shaderPaths, const std::string subDirectory, const std::vector<std::string>& defines);
		bool Load(std::vector<std::string> shaderPaths);
		bool HotLoad();

		uint32_t GetHandle() { return m_Handle; }
		int32_t GetUniformLocation(const std::string& name);
		const std::vector<std::string>& GetPaths() const { return m_ShaderPaths; }
		size_t GetCPUAllocatedByteCount() const;

	private:
		uint32_t m_Handle = 0;
		std::vector<std::string> m_Defines;
		std::vector<std::string> m_ShaderPaths;
		std::unordered_map<std::string, int32_t> m_UniformLocations;
		std::string m_SubDirectory = "";
	};
}
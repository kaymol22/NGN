#include "OpenGLShader.h"

#include <fstream>
#include <sstream>

namespace NGN
{
	static GLenum ShaderStageToOpenGL(const std::string& type)
	{
		if (type == "vertex")							return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")		return GL_FRAGMENT_SHADER;
		if (type == "compute")							return GL_COMPUTE_SHADER;

		NGN_CORE_ASSERT(false, "Unknown shader stage");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::filesystem::path& filePath)
	{
		m_SourcePaths[GL_COMPUTE_SHADER] = filePath;

		std::string source = ReadTextFile(filePath);
		auto shaderSources = Preprocess(source);
		CompileAndLink(shaderSources);

		m_Name = filePath.stem().string();
	}

	OpenGLShader::OpenGLShader(
		const std::string& name,
		const std::filesystem::path& vertexSrc,
		const std::filesystem::path& fragmentSrc)
		: m_Name(name)
	{
		m_SourcePaths[GL_VERTEX_SHADER] =	vertexSrc;
		m_SourcePaths[GL_FRAGMENT_SHADER] =	fragmentSrc;

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = ReadTextFile(vertexSrc);
		sources[GL_FRAGMENT_SHADER] = ReadTextFile(fragmentSrc);

		CompileAndLink(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadTextFile(const std::filesystem::path& filePath)
	{
		std::ifstream file(filePath);

		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << filePath.string() << std::endl;
			return {};
		}

		std::stringstream contentStream;
		contentStream << file.rdbuf();
		return contentStream.str();
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::Reload()
	{
		if (m_SourcePaths.empty())
			return;
		
		GLuint oldProgram = m_RendererID;

		std::unordered_map<GLenum, std::string> sources;
		for (auto& [stage, path] : m_SourcePaths)
			sources[stage] = ReadTextFile(path);
		
		CompileAndLink(sources);

		if (m_RendererID != oldProgram)
			glDeleteProgram(oldProgram);
	}

	std::unordered_map<GLenum, std::string>
		OpenGLShader::Preprocess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> result;

		const char* token = "#type";
		size_t pos = source.find(token, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			size_t begin = pos + strlen(token) + 1;
			std::string type = source.substr(begin, eol - begin);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(token, nextLinePos);

			result[ShaderStageToOpenGL(type)] =
				source.substr(
					nextLinePos,
					pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)
				);
		}
		
		return result;
	}

	void OpenGLShader::CompileAndLink(const std::unordered_map<GLenum, std::string>& sources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLuint> shaderIDs;

		for (auto& [stage, src] : sources)
		{
			GLuint shader = glCreateShader(stage);
			const GLchar* source = src.c_str();
			glShaderSource(shader, 1, &source, nullptr);
			glCompileShader(shader);

			GLint isCompiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				NGN_CORE_ERROR("Shader compile error: \n{}", infoLog.data());

				glDeleteShader(shader);
				return;
			}

			glAttachShader(program, shader);
			shaderIDs.push_back(shader);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			NGN_CORE_ERROR("Shader compile error: \n{}", infoLog.data());

			glDeleteProgram(program);
			return;
		}

		for (auto id : shaderIDs)
			glDeleteShader(id);

		m_RendererID = program;
	}
}
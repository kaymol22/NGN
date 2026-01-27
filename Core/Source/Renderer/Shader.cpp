#include "Core/ngnpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/gl.h>

#include <fstream>


namespace Renderer {

	static std::string ReadTextFile(const std::filesystem::path& path)
	{
		std::ifstream file(path);

		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << path.string() << std::endl;
			return {};
		}

		std::stringstream contentStream;
		contentStream << file.rdbuf();
		return contentStream.str();
	}

	uint32_t CreateComputeShader(const std::filesystem::path& path)
	{
		std::string shaderSource = ReadTextFile(path);

		GLuint shaderHandle = glCreateShader(GL_COMPUTE_SHADER);

		const GLchar* source = (const GLchar*)shaderSource.c_str();
		glShaderSource(shaderHandle, 1, &source, 0);

		glCompileShader(shaderHandle);

		GLint isCompiled = 0;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteShader(shaderHandle);
			return 0;
		}

		GLuint program = glCreateProgram();
		glAttachShader(program, shaderHandle);
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteProgram(program);
			glDeleteShader(shaderHandle);

			return 0;
		}

		glDetachShader(program, shaderHandle);
		return program;
	}

	uint32_t ReloadComputeShader(uint32_t shaderHandle, const std::filesystem::path& path)
	{
		uint32_t newShaderHandle = CreateComputeShader(path);

		// Return old shader if compilation failed
		if (newShaderHandle == 0)
			return shaderHandle;

		glDeleteProgram(shaderHandle);
		return newShaderHandle;
	}

	uint32_t CreateGraphicsShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{
		std::string vertexShaderSource = ReadTextFile(vertexPath);
		std::string fragmentShaderSource = ReadTextFile(fragmentPath);

		// Vertex Shader 

		GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = (const GLchar*)vertexShaderSource.c_str();
		glShaderSource(vertexShaderHandle, 1, &source, 0);

		glCompileShader(vertexShaderHandle);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShaderHandle, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteShader(vertexShaderHandle);
			return 0;
		}

		// Fragment Shader 

		GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

		source = (const GLchar*)fragmentShaderSource.c_str();
		glShaderSource(fragmentShaderHandle, 1, &source, 0);

		glCompileShader(fragmentShaderHandle);

		isCompiled = 0;
		glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShaderHandle, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteShader(fragmentShaderHandle);
			return 0;
		}

		// Program Linking

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShaderHandle);
		glAttachShader(program, fragmentShaderHandle);
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteProgram(program);
			glDeleteShader(vertexShaderHandle);
			glDeleteShader(fragmentShaderHandle);

			return 0;
		}

		glDetachShader(program, vertexShaderHandle);
		glDetachShader(program, fragmentShaderHandle);
		return program;
	}

	uint32_t ReloadGraphicsShader(uint32_t shaderHandle, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{
		uint32_t newShaderHandle = CreateGraphicsShader(vertexPath, fragmentPath);

		// Return old shader if compilation failed
		if (newShaderHandle == 0)
			return shaderHandle;

		glDeleteProgram(shaderHandle);
		return newShaderHandle;
	}
}

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
#pragma once
#include "Platform/OpenGL/Resources/GL_Framebuffer.h"
//#include "Types/GL_Shader.h"

namespace OpenGL::Commands
{
	// Shaders
	void BindShader(const std::string& name);
	void UnbindShader();
	void SetInt(const std::string& name, int value);
	void SetIntArray(const std::string& name, int* values, uint32_t count);
	void SetFloat(const std::string& name, float value);
	void SetFloat2(const std::string& name, const glm::vec2& value);
	void SetFloat3(const std::string& name, const glm::vec3& value);
	void SetFloat4(const std::string& name, const glm::vec4& value);
	void SetMat3(const std::string& name, const glm::mat3& value);
	void SetMat4(const std::string& name, const glm::mat4& value);

	// SSBO's

	// Textures

	// Blitting
}
#pragma once
#include "Platform/OpenGL/Resources/GL_Framebuffer.h"
//#include "Types/GL_Shader.h"

namespace OpenGL::Commands
{
	void Clear();
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
	void BindSSBO(uint32_t bindingIndex, const std::string& ssboName);
	void BindSSBO(uint32_t bindingIndex, uint32_t ssboHandle);
	void ClearSSBO(const std::string& name);


	// Textures
	void BindImageTexture(uint32_t bindingIndex, uint32_t textureHandle, uint32_t access, uint32_t format, bool layered = false);
	void BindImageTextureArray(uint32_t bindingindex, uint32_t textureHandle, uint32_t access, uint32_t format);
	void BindTextureUnit(uint32_t bindingIndex, uint32_t textureHandle);

	// Blitting
}
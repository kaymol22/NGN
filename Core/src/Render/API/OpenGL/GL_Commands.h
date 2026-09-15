#pragma once
#include "GL_ResourceManager.h"
#include "Render/RendererTypes.h"

namespace OpenGL::Commands
{
	void Clear();
	// Shaders
	void BindShader(const std::string& name);
	void UnbindShader();
	void SetUniformBool(const std::string& name, bool value);
	void SetUniformInt(const std::string& name, int value);
	void SetUniformUInt(const std::string& name, uint32_t value);
	void SetUniformFloat(const std::string& name, float value);
	void SetUniformMat2(const std::string& name, const glm::mat3& value);
	void SetUniformMat3(const std::string& name, const glm::mat3& value);
	void SetUniformMat4(const std::string& name, const glm::mat4& value);
	void SetUniformIVec2(const std::string& name, const glm::ivec2& value);
	void SetUniformUVec2(const std::string& name, const glm::uvec2& value);
	void SetUniformVec2(const std::string& name, const glm::vec2& value);
	void SetUniformVec2(const std::string& name, float x, float y);
	void SetUniformVec3(const std::string& name, const glm::vec3& value);
	void SetUniformVec3(const std::string& name, float x, float y, float z);
	void SetUniformIVec3(const std::string& name, const glm::ivec3& value);
	void SetUniformUVec3(const std::string& name, const glm::uvec3& value);
	void SetUniformVec4(const std::string& name, const glm::vec4& value);
	void SetUniformVec4(const std::string& name, float x, float y, float z, float w);
	void SetUniformVec2Array(const std::string& name, const std::vector<glm::vec2>& data);
	void SetUniformIVec2Array(const std::string& name, const std::vector<glm::ivec2>& data);

	// SSBO's
	void BindSSBO(uint32_t bindingIndex, const std::string& ssboName);
	void BindSSBO(uint32_t bindingIndex, uint32_t vboHandle);
	void ClearSSBO(const std::string& name);
	void ReserveSSBO(const std::string& name, size_t size);
	void UpdateSSBO(const std::string& name, size_t size, const void* data);
	void UpdateSSBORange(const std::string& name, size_t offset, size_t size, const void* data);
	void UploadSSBOStatic(const std::string& name, size_t size, const void* data);


	// Textures
	void BindImageTexture(uint32_t bindingIndex, uint32_t textureHandle, uint32_t access, uint32_t format, bool layered = false);
	void BindImageTextureArray(uint32_t bindingindex, uint32_t textureHandle, uint32_t access, uint32_t format);
	void BindTextureUnit(uint32_t bindingIndex, uint32_t textureHandle);

	// Blitting
	void BlitFrameBuffer(OpenGLFrameBuffer* src, OpenGLFrameBuffer* dst, const char* srcName, const char* dstName, GLbitfield mask, GLenum filter);
	void BlitFrameBuffer(OpenGLFrameBuffer* src, OpenGLFrameBuffer* dst, const char* srcName, const char* dstName, BlitRect srcRect, BlitRect dstRect, GLbitfield mask, GLenum filter);
	void BlitToDefaultFrameBuffer(OpenGLFrameBuffer* srcFrameBuffer, const char* srcName, GLbitfield mask, GLenum filter);
	void BlitToDefaultFrameBuffer(OpenGLFrameBuffer* srcFrameBuffer, const char* srcName, BlitRect srcRect, BlitRect dstRect, GLbitfield mask, GLenum filter);
}
#include "GL_Commands.h"
#include "GL_Backend.h"

namespace OpenGL::Commands
{
	namespace {
		OpenGLShader* g_BoundShader = nullptr;
	}
	void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void BindShader(const std::string& name) {
		OpenGLShader* shader = OpenGL::ResourceManager::GetShaderPtr(name);
		if (!shader) {
			NGN_CORE_ERROR("Commands::BindShader - Shader '{}' not found", name);
			return;
		}

		if (g_BoundShader == shader) {
			NGN_CORE_INFO("Commands::BindShader - Shader '{}' already bound", name);
			return;
		}

		g_BoundShader = shader;
		glUseProgram(g_BoundShader->GetHandle());
	}
	void BindImageTexture(uint32_t bindingIndex, uint32_t textureHandle, uint32_t access, uint32_t format, bool layered)
	{
		glBindImageTexture(
			static_cast<GLuint>(bindingIndex),
			static_cast<GLuint>(textureHandle),
			0,
			layered,
			0,
			static_cast<GLenum>(access),
			static_cast<GLenum>(format)
		);
	}

	void BindImageTextureArray(uint32_t bindingIndex, uint32_t textureHandle, uint32_t access, uint32_t format)
	{
		glBindImageTexture(
			static_cast<GLuint>(bindingIndex),
			static_cast<GLuint>(textureHandle),
			0,
			GL_TRUE,
			0,
			static_cast<GLenum>(access),
			static_cast<GLenum>(format)
		);
	}

	void BindTextureUnit(uint32_t bindingIndex, uint32_t textureHandle)
	{
		glBindTextureUnit(static_cast<GLuint>(bindingIndex), static_cast<GLuint>(textureHandle));
	}
}
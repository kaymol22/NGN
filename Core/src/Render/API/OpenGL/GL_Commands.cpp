#include "GL_Commands.h"
#include "GL_Backend.h"

namespace OpenGL::Commands
{
	void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
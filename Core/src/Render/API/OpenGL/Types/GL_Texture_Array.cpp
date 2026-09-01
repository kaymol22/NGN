#include "GL_Texture_Array.h"
#include "Render/API/OpenGL/GL_Utils.h"

#include <glad/gl.h>

namespace OpenGL
{
	void TextureArray::AllocateMemory(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t mipmapLevelCount, uint32_t count)
	{
		if (m_MemoryAllocated) return;

		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_Handle);
		glTextureStorage3D(m_Handle, mipmapLevelCount, internalFormat, width, height, count);
		glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_R, GL_REPEAT);

		m_Width = width;
		m_Height = height;
		m_InternalFormat = internalFormat;
		m_MipMapLevelCount = mipmapLevelCount;
		m_Count = count;
		m_MemoryAllocated = true;
		m_Format = Utils::GLInternalFormatToGLFormat(m_InternalFormat);
		m_Type = Utils::GLInternalFormatToGLType(m_InternalFormat);
	}

	void TextureArray::CleanUp()
	{
		if (m_Handle != 0) {
			glDeleteTextures(1, &m_Handle);
			m_Handle = 0;
		}

		m_Width = 0;
		m_Height = 0;
		m_Count = 0;
		m_Format = 0;
		m_InternalFormat = 0;
		m_MipMapLevelCount = 0;
		m_Type = 0;
		m_MemoryAllocated = false;
	}
}
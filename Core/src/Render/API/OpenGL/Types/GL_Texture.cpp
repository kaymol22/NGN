#include "GL_Texture.h"
#include "Render/API/OpenGL/GL_Utils.h"
#include <glad/gl.h>
#include <stb_image.h>

namespace OpenGL
{
	void OpenGL::OpenGLTexture::Create(int width, int height, int internalFormat, int mipmapLevelCount)
	{
		if (m_Handle != 0 || m_BindlessId != 0) {
			Reset();
		}
		m_Width = width;
		m_Height = height;
		m_MipMapLevelCount = mipmapLevelCount;
		m_InternalFormat = internalFormat;
		m_Format = Utils::GetFormatFromInternalFormat(internalFormat);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
		glTextureStorage2D(m_Handle, mipmapLevelCount, internalFormat, width, height);

		SetMinFilter(mipmapLevelCount > 1 ? TextureFilter::LINEAR_MIPMAP : TextureFilter::LINEAR);
		SetMagFilter(TextureFilter::LINEAR);
		SetWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
	}

	void OpenGLTexture::UploadData(const float* data)
	{
		if (!m_Handle || !data) return;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

		glTextureSubImage2D(m_Handle, 0, 0, 0, m_Width, m_Height, m_Format, GL_FLOAT, data);
	}

	GLuint& OpenGLTexture::GetHandle() {
		return m_Handle;
	}
	
}
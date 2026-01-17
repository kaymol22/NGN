#include "OpenGLTexture.h"
#include <glad/gl.h>
#include <stb_image.h>

namespace NGN
{
	OpenGLTexture::OpenGLTexture(const TextureSpecification& specification)
		: m_Width(specification.Width), m_Height(specification.Height)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		// Mapping engine format to OpenGL format
		GLenum internalFormat = GL_RGBA8;
		GLenum dataFormat = GL_RGBA;

		switch (specification.Format)
		{
		case ImageFormat::RGBA8:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			break;
		case ImageFormat::R8:
			internalFormat = GL_R8;
			dataFormat = GL_RED;
			break;
		case ImageFormat::RGB8:
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			break;
		case ImageFormat::RGBA32F:
			internalFormat = GL_RGBA32F;
			dataFormat = GL_RGBA;
			break;
		default:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		// set default params
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture::OpenGLTexture(const std::string& filePath)
		: m_Path(filePath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

		if (!data)
		{
			// TODO: Implement NGN_CORE_ASSERT here
			m_Width = m_Height = 0;
			return;
		}

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) { internalFormat = GL_RGBA8; dataFormat = GL_RGBA; }
		else if (channels == 3) { internalFormat = GL_RGB8; dataFormat = GL_RGB; }
		else if (channels == 1) { internalFormat = GL_R8; dataFormat = GL_RED; }

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		// set default params
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (true) // TODO: check specification.GenerateMipMaps
			glGenerateTextureMipmap(m_RendererID);

		stbi_image_free(data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}
}
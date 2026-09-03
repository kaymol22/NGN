#include "GL_Texture.h"
#include "Render/API/OpenGL/GL_Utils.h"
#include <glad/gl.h>
#include <stb_image.h>

namespace OpenGL
{
	void OpenGLTexture::Create(int width, int height, int internalFormat, int mipmapLevelCount)
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

	void OpenGLTexture::UploadPixelData(const void* data, size_t dataSize, GLenum sourceType, int mipLevel)
	{
		if (!m_Handle || !data) return;

		const int divisor = 1 << mipLevel;
		const int mipWidth = m_Width > divisor ? m_Width / divisor : 1;
		const int mipHeight = m_Height > divisor ? m_Height / divisor : 1;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

		m_DataSize = static_cast<GLsizei>(dataSize);

		glTextureSubImage2D(m_Handle, mipLevel, 0, 0, mipWidth, mipHeight, m_Format, sourceType, data);
	}

	void OpenGLTexture::UploadR16FData(const float* data, int width, int height, int xOffset, int yOffset, int mipLevel)
	{
		if (!m_Handle || !data) return;

		if (m_InternalFormat != GL_R16F)
		{
			NGN_CORE_ERROR("UploadR16FData failed: m_InternalFormat is {}", Utils::GLInternalFormatToString(m_InternalFormat));
			return;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

		glTextureSubImage2D(m_Handle, mipLevel, xOffset, yOffset, width, height, GL_RED, GL_FLOAT, data);
	}

	void OpenGLTexture::GenerateMipMaps()
	{
		if (!m_Handle) return;
		glGenerateTextureMipmap(m_Handle);
	}

	void OpenGLTexture::ClearR(float value)
	{
		if (!m_Handle) return;

		if (!(m_Format == GL_RED || m_Format == GL_RG || m_Format == GL_RGB || m_Format == GL_RGBA)) {
			NGN_CORE_WARN("OpenGLTexture::ClearR() Unsupported format");
			return;
		}

		const GLfloat color[4] = { value, 0.0f, 0.0f, 0.0f };
		for (int level = 0; level < m_MipMapLevelCount; ++level) {
			glClearTexImage(m_Handle, level, m_Format, GL_FLOAT, color);
		}
	}

	void OpenGLTexture::Reset()
	{
		if (m_BindlessId != 0 && m_BindlessResident)
		{
			MakeBindlessTextureNonResident();
		}

		if (m_Handle != 0)
		{
			glDeleteTextures(1, &m_Handle);
			m_Handle = 0;
		}

		m_BindlessId = 0;
		m_Width = 0;
		m_Height = 0;
		m_ChannelCount = 0;
		m_DataSize = 0;
		m_Data = nullptr;
		m_Format = 0;
		m_InternalFormat = 0;
		m_MipMapLevelCount = 0;
		m_ImageDataType = ImageDataType::UNCOMPRESSED;
	}

	void OpenGLTexture::SetBorderColor(float r, float g, float b, float a)
	{
		if (!m_Handle) return;
		float color[4] = { r, g, b, a };
		glTextureParameterfv(m_Handle, GL_TEXTURE_BORDER_COLOR, color);
	}

	void OpenGLTexture::SetBorderColor(glm::vec4 color)
	{
		SetBorderColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLTexture::SetWrapMode(TextureWrapMode wrapMode)
	{
		SetWrapModeS(wrapMode);
		SetWrapModeT(wrapMode);
	}

	void OpenGLTexture::SetWrapModeS(TextureWrapMode wrapMode)
	{
		if (!m_Handle) return;
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, Utils::TextureWrapModeToGLEnum(wrapMode));
	}

	void OpenGLTexture::SetWrapModeT(TextureWrapMode wrapMode)
	{
		if (!m_Handle) return;
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, Utils::TextureWrapModeToGLEnum(wrapMode));
	}

	void OpenGLTexture::SetMinFilter(TextureFilter filter)
	{
		if (!m_Handle) return;
		glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, Utils::TextureFilterToGLEnum(filter));
	}

	void OpenGLTexture::SetMagFilter(TextureFilter filter)
	{
		if (!m_Handle) return;
		glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, filter == TextureFilter::NEAREST ? GL_NEAREST : GL_LINEAR);
	}

	void OpenGLTexture::MakeBindlessTextureResident()
	{
		if (!m_Handle || m_BindlessResident) return;

		if (m_BindlessId == 0)
		{
			m_BindlessId = glGetTextureHandleARB(m_Handle);
		}

		glMakeTextureHandleResidentARB(m_BindlessId);
		m_BindlessResident = true;
	}

	void OpenGLTexture::MakeBindlessTextureNonResident()
	{
		if (m_BindlessId == 0 || !m_BindlessResident) return;

		glMakeTextureHandleNonResidentARB(m_BindlessId);
		m_BindlessResident = false;
	}

	size_t OpenGLTexture::GetAllocatedByteCount() const
	{
		return Utils::CalculateTexture2DByteCount(m_Width, m_Height, m_InternalFormat, m_MipMapLevelCount);
	}
}
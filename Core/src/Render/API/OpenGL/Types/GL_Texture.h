#pragma once

#include "Render/TextureTypes.h"
#include <glad/gl.h>

namespace OpenGL
{
	struct OpenGLTexture {
		OpenGLTexture() = default;
		GLuint& GetHandle();
		GLuint64 GetBindlessId() { return m_BindlessId; }
		void Create(int width, int height, int internalFormat, int mipmapLevelCount);
		void ClearR(float value);
		void UploadData(const float* data);
		void UploadR16FData(const float* data, int width, int height, int xOffset, int yOffset, int mipLevel);
		void Reset();
		void SetBorderColor(float r, float g, float b, float a);
		void SetBorderColor(glm::vec4 color);
		void SetWrapMode(TextureWrapMode wrapMode);
		void SetWrapModeS(TextureWrapMode wrapMode);
		void SetWrapModeT(TextureWrapMode wrapMode);
		void SetMinFilter(TextureFilter filter);
		void SetMagFilter(TextureFilter filter);
		void MakeBindlessTextureResident();
		void MakeBindlessTextureNonResident();
		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }
		int GetChannelCount() { return m_ChannelCount; }
		int GetDataSize() { return m_DataSize; }
		size_t GetGPUAllocatedByteCount() const;
		void* GetData() { return m_Data; }
		GLint GetFormat() { return m_Format; }
		GLint GetInternalFormat() { return m_InternalFormat; }
		GLint GetMipMapLevelCount() { return m_MipMapLevelCount; }
		
	private:
		GLuint m_Handle = 0;
		GLuint64 m_BindlessId = 0;
		int m_Width = 0;
		int m_Height = 0;
		int m_ChannelCount = 0;
		GLsizei m_DataSize = 0;
		void* m_Data = nullptr;
		GLint m_Format = 0;
		GLint m_InternalFormat = 0;
		GLint m_MipMapLevelCount = 0;
		ImageDataType m_ImageDataType = ImageDataType::UNCOMPRESSED;
	};
}
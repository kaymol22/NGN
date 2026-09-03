#pragma once

#include "Render/TextureTypes.h"

#include <glad/gl.h>

namespace OpenGL::Utils {

	const char* GLDebugSourceToString(GLenum source);
	const char* GLDebugTypeToString(GLenum type);
	const char* GLDebugSeverityToString(GLenum severity);

	void InitOpenGLDebugMessageCallback();

	GLint GetFormatFromInternalFormat(GLint internalFormat);
	GLenum GLInternalFormatToGLType(GLenum internalFormat);
	GLenum GLInternalFormatToGLFormat(GLenum internalFormat);
	const char* GLInternalFormatToString(GLenum internalFormat);
	
	GLenum ImageFormatToGLInternalFormat(ImageFormat format);
	GLint TextureWrapModeToGLEnum(TextureWrapMode wrapMode);
	GLint TextureFilterToGLEnum(TextureFilter filter);

	// TODO: Find a way to get actual sample count into this func call
	size_t CalculateTexture2DByteCount(uint32_t width, uint32_t height, GLenum internalFormat, uint32_t mipmapLevelCount, uint32_t sampleCount = 1);
	size_t GetBytesPerPixel(GLenum internalFormat);
	size_t GetCompressedBlockSize(GLenum internalFormat);
}
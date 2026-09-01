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
}
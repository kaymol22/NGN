#pragma once

#include <glad/gl.h>

namespace GLUtils {

	const char* GLDebugSourceToString(GLenum source);
	const char* GLDebugTypeToString(GLenum type);
	const char* GLDebugSeverityToString(GLenum severity);

	void InitOpenGLDebugMessageCallback();
}
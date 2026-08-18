#pragma once
#include <glad/gl.h>

namespace NGN
{
	struct OpenGLRasterizerState
	{
		// Blending
		GLboolean blendEnable = GL_FALSE;
		GLenum blendSrcFactor = GL_SRC_ALPHA;
		GLenum blendDstFactor = GL_ONE_MINUS_SRC_ALPHA;

		// Colour
		GLboolean colorMask = GL_TRUE;

		// Depth
		GLboolean depthTestEnable = GL_TRUE;
		GLboolean depthMask = GL_TRUE;
		GLenum depthFunc = GL_LESS;

		// Culling
		GLboolean cullFaceEnable = GL_TRUE;
		GLenum cullFaceMode = GL_BACK;

		// Stencil
		GLboolean stencilTestEnable = GL_FALSE;
		GLenum stencilFunc = GL_ALWAYS;
		GLint stencilRef = 0;
		GLuint stencilReadMask = 0xFF;
		GLuint stencilWriteMask = 0xFF;
		GLenum stencilFailOp = GL_KEEP;
		GLenum stencilDepthFailOp = GL_KEEP;
		GLenum stencilPassOp = GL_KEEP;

		// Misc
		GLfloat pointSize = 1.0f;

		void Apply() const;
	};
}
#include "GL_GeometryPass.h"
#include <glad/gl.h>

namespace NGN
{
	OpenGLGeometryPass::OpenGLGeometryPass()
	{
		// Opaque, depth write, no blend
		m_RasterizerState.blendEnable = GL_FALSE;
		m_RasterizerState.depthMask = GL_TRUE;
		m_RasterizerState.cullFaceEnable = GL_TRUE;
	}

	void OpenGLGeometryPass::Render(Ref<Framebuffer> target)
	{
		m_RasterizerState.Apply();
		target->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (const auto& item : m_Items)
		{
			// bind mesh, shader, draw using item data
		}

		target->Unbind();
	}
}
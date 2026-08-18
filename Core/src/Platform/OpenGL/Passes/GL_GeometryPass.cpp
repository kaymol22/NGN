#include "GL_GeometryPass.h"
#include <glad/gl.h>

#include "Scene/Scenebvh.h"

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
		NGN_CORE_INFO("Executing OpenGLGeometryPass with {0} items", m_Items.size());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (const auto& item : m_Items)
		{
			NGN_CORE_INFO("Rendering item with mesh index: {0}, object type: {1}", item.meshIndex, item.objectType);
			// bind mesh, shader, draw using item data
		}

		target->Unbind();
	}
}
#pragma once
#include "Renderer/Passes/GeometryPass.h"
#include "GL_RasterizerState.h"

namespace NGN
{
	class OpenGLGeometryPass : public GeometryPass
	{
	public:
		OpenGLGeometryPass();
	protected:
		void Render(Ref<Framebuffer> target) override;
	private:
		OpenGLRasterizerState m_RasterizerState;
	};
}
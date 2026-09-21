#include "Render/API/OpenGL/GL_Renderer.h"
#include "Render/API/OpenGL/GL_Commands.h"

namespace OpenGL::Renderer
{
	void RenderDeferred()
	{
		ClearRenderTargets();
		UpdateSSBOs();

		GeometryPass();
		/*TestPass();*/
		OpenGLFrameBuffer& gBuffer = ResourceManager::GetFrameBuffer("gBuffer");
		PresentFinalImage(gBuffer);
	}

	void ClearRenderTargets()
	{
		OpenGLFrameBuffer* gBuffer = ResourceManager::GetFrameBufferPtr("gBuffer");
		gBuffer->Bind();
		gBuffer->ClearAttachment("BaseColorMetallic", 0.0f, 0.0f, 0.0f, 0.0f);
		gBuffer->ClearAttachment("NormalRoughness", 0.0f, 0.0f, 0.0f, 1.0f);
		gBuffer->ClearDepthAttachment(1.0f);
		gBuffer->ClearStencilBits(0.0f);

		OpenGLFrameBuffer* presentFBO = ResourceManager::GetFrameBufferPtr("Present");
		presentFBO->Bind();
		presentFBO->ClearAttachment("FinalColor", 0.1f, 0.1f, 0.1f, 0.0f);
	}
}
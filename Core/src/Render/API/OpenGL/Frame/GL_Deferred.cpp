#include "Passes/GL_GeometryPass.cpp"
#include "Render/API/OpenGL/GL_Renderer.h"
#include "Render/API/OpenGL/GL_Commands.h"

namespace OpenGL::Renderer
{
	void ClearRenderTargets();

	void RenderScene()
	{
		GeometryPass();

		OpenGLFrameBuffer& gBuffer = ResourceManager::GetFrameBuffer("GBuffer");
		OpenGLFrameBuffer& presentFBO = ResourceManager::GetFrameBuffer("Present");

		Commands::BlitFrameBuffer(&gBuffer, &presentFBO, "Color", "Color", GL_COLOR_BUFFER_BIT, GL_NEAREST);
		PresentFinalImage(presentFBO);
	}

	void ClearRenderTargets()
	{
		OpenGLFrameBuffer* gBuffer = ResourceManager::GetFrameBufferPtr("GBuffer");
		gBuffer->Bind();
		gBuffer->ClearAttachment("BaseColor+Config", 0.0f, 0.0f, 0.0f, 1.0f);
		gBuffer->ClearAttachment("Lighting", 0.0f, 0.0f, 0.0f, 1.0f);
	}
}
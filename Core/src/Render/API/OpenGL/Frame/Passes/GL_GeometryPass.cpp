#include "Render/FrameManager.h"
#include "Render/API/OpenGL/GL_ResourceManager.h"
#include "Render/API/OpenGL/GL_Commands.h"

namespace OpenGL::Renderer
{
	void GeometryPass()
	{
		NGN_PROFILE_FUNCTION();

		const DrawCommandsSet& drawInfoSet = NGN::Renderer::FrameManager::GetDrawInfoSet();
		const ViewportData& viewportData = NGN::Renderer::FrameManager::GetViewportData();

		OpenGLFrameBuffer* gBuffer = ResourceManager::GetFrameBufferPtr("GBuffer");
		OpenGLShader* shader = ResourceManager::GetShaderPtr("GeometryPass");

		if (!gBuffer) return;
		if (!shader) return;

		Commands::BindShader("GBuffer");
		Commands::BindSSBO(0, "ViewportData");

		gBuffer->Bind();
		gBuffer->DrawBuffers({ "BaseColor+Config", "Lighting" });
	}
}
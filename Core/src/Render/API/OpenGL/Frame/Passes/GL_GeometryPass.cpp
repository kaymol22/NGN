#include "Render/FrameManager.h"
#include "Render/API/OpenGL/GL_ResourceManager.h"
#include "Render/API/OpenGL/GL_Commands.h"
#include "Render/API/OpenGL/GL_Renderer.h"
#include "Render/API/OpenGL/GL_Rasterizer_State_Manager.h"

namespace OpenGL::Renderer
{
	void GeometryPass()
	{
		NGN_PROFILE_FUNCTION();

		OpenGLFrameBuffer* gBuffer = ResourceManager::GetFrameBufferPtr("gBuffer");
		OpenGLShader* shader = ResourceManager::GetShaderPtr("gBuffer");

		if (!gBuffer) { NGN_CORE_INFO("GeometryPass: Couldnt get gBuffer"); return; }
		if (!shader) { NGN_CORE_INFO("GeometryPass: Couldnt get Shader"); return; }

		Commands::BindShader("gBuffer");
		Commands::BindSSBO(SSBO_IDX_VIEWPORT_DATA, "ViewportData");
		Commands::BindSSBO(SSBO_IDX_SCENE_RENDER_ITEMS, "SceneRenderItems");
		Commands::BindSSBO(SSBO_IDX_DRAW_RENDER_ITEM_INDICES, "DrawRenderItemIndices");

		glViewport(0, 0, gBuffer->GetWidth(), gBuffer->GetHeight());

		OpenGLMeshBuffer& meshBuffer = ResourceManager::GetMeshBuffer("AssetGeometry");
		glBindVertexArray(meshBuffer.GetVAO());
		

		gBuffer->Bind();
		gBuffer->DrawBuffers({ "BaseColorMetallic", "NormalRoughness" });

		RasterizerStateManager::ForceRasterizerState("GeometryPass");

		const DrawCommandsSet& drawInfoSet = NGN::Renderer::FrameManager::GetDrawInfoSet();
		MultiDrawIndirect(drawInfoSet.standard);
	}
}
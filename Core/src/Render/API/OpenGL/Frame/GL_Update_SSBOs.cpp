#include "../GL_Renderer.h"
#include "../GL_Commands.h"
#include "Render/FrameManager.h"

namespace OpenGL::Renderer
{
	namespace {
		template<typename T>
		void UpdateSSBOValue(const std::string& ssboName, const T& value) {
			Commands::UpdateSSBO(ssboName, sizeof(T), &value);
		}

		template<typename T>
		void UpdateSSBOVector(const std::string& ssboName, const std::vector<T>& vector) {
			if (vector.empty()) return;
			Commands::UpdateSSBO(ssboName, vector.size() * sizeof(T), vector.data());
		}
	}

	void UpdateSSBOs()
	{
		UpdateSSBOValue("ViewportData", NGN::Renderer::FrameManager::GetViewportData());

		UpdateSSBOVector("SceneRenderItems", NGN::Renderer::FrameManager::GetSceneGPUData());
		UpdateSSBOVector("DrawRenderItemIndices", NGN::Renderer::FrameManager::GetDrawRenderItemIndices());

		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		Commands::BindSSBO(SSBO_IDX_VIEWPORT_DATA, "ViewportData");
		Commands::BindSSBO(SSBO_IDX_SCENE_RENDER_ITEMS, "SceneRenderItems");
		Commands::BindSSBO(SSBO_IDX_DRAW_RENDER_ITEM_INDICES, "DrawRenderItemIndices");
	}
}
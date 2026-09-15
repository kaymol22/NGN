#include "Render/API/OpenGL/GL_ResourceManager.h"
#include "Render/RendererTypes.h"

namespace OpenGL::Renderer
{
	void CreateSSBOs() {
		int dummySize = 64;

		OpenGL::ResourceManager::CreateSSBO("ViewportData").Create(sizeof(ViewportData), GL_DYNAMIC_STORAGE_BIT);
		OpenGL::ResourceManager::CreateSSBO("RendererData").Create(sizeof(RendererData), GL_DYNAMIC_STORAGE_BIT);
		OpenGL::ResourceManager::CreateSSBO("SceneRenderItems").Create(dummySize, GL_DYNAMIC_STORAGE_BIT);
		OpenGL::ResourceManager::CreateSSBO("DrawRenderItemIndices").Create(dummySize, GL_DYNAMIC_STORAGE_BIT);
	}
}
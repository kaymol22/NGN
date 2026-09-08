#include "Render/API/OpenGL/GL_ResourceManager.h"

namespace OpenGL::Renderer
{
	static void CreateSSBOs() {
		int dummySize = 64;

		OpenGL::ResourceManager::CreateSSBO("ViewportData");
		OpenGL::ResourceManager::CreateSSBO("RendererData");
	}
}
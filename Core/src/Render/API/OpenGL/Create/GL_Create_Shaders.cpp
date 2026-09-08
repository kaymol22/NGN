#include "Render/API/OpenGL/GL_ResourceManager.h"

namespace OpenGL::Renderer
{
	static void CreateShaders()
	{
		OpenGL::ResourceManager::LoadShader("GBuffer", { "GL_gbuffer.vert", "GL_gbuffer.frag" });
		OpenGL::ResourceManager::LoadShader("Present", { "GL_present.vert", "GL_present.frag" });
	}
}
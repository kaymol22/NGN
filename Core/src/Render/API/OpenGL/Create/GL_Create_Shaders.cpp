#include "Render/API/OpenGL/GL_Renderer.h"
#include "Render/API/OpenGL/GL_ResourceManager.h"
// Debug - remove me
#include <GLFW/glfw3.h>

namespace OpenGL::Renderer
{
	void CreateShaders()
	{
		OpenGL::ResourceManager::LoadShader("Test", { "GL_test.vert", "GL_test.frag" });
		OpenGL::ResourceManager::LoadShader("gBuffer", { "GL_gbuffer.vert", "GL_gbuffer.frag" });
		OpenGL::ResourceManager::LoadShader("Present", { "GL_present.vert", "GL_present.frag" });
	}
}
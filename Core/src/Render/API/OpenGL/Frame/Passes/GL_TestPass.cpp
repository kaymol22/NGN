#include "Render/API/OpenGL/GL_Renderer.h"
#include "Render/API/OpenGL/GL_ResourceManager.h"
#include "Render/API/OpenGL/GL_Commands.h"
#include "Render/API/OpenGL/GL_Rasterizer_State_Manager.h"

#include <GLFW/glfw3.h>

namespace OpenGL::Renderer
{
	void TestPass() {
		OpenGLFrameBuffer* gBuffer = ResourceManager::GetFrameBufferPtr("gBuffer");
		if (!gBuffer) { NGN_CORE_INFO("TestPass: Couldnt get gBuffer"); return; }

		Commands::BindShader("Test");

		gBuffer->Bind();
		gBuffer->DrawBuffers({ "BaseColorMetallic", "NormalRoughness" });
		glViewport(0, 0, gBuffer->GetWidth(), gBuffer->GetHeight());

		OpenGLRasterizerState state;
		state.depthTestEnable = false;
		state.blendEnable = false;
		state.cullFaceEnable = false;
		state.depthMask = false;
		state.colorMask = true;
		RasterizerStateManager::ForceRasterizerState(state);

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		printf("Viewport: x=%d, y=%d, width=%d, height=%d\n",
			viewport[0], viewport[1], viewport[2], viewport[3]);

		BindEmptyVAO();
		OpenGLShader* shader = ResourceManager::GetShaderPtr("Test");
		std::cout << "Test handle " << shader->GetHandle() << " isProgram " << static_cast<int>(glIsProgram(shader->GetHandle())) << "\n";

		int pathCount = shader->GetPaths().size();
		for (int i = 0; i < pathCount; ++i) {
			NGN_CORE_INFO("Test shader path[{}]: {}", i, shader->GetPaths()[i]);
		}

		GLint p = 0; glGetIntegerv(GL_CURRENT_PROGRAM, &p);
		NGN_CORE_INFO("TestPass: GL_CURRENT_PROGRAM = {}", p);

		/*NGN_CORE_INFO("ctx {} | GL_VERSION {}", (void*)glfwGetCurrentContext(),
			(const char*)glGetString(GL_VERSION));*/

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
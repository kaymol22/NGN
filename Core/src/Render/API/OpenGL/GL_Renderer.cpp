#include "GL_Renderer.h"
#include "GL_Backend.h"

#include "Create/GL_Create_FrameBuffers.cpp"
#include "Create/GL_Create_Shaders.cpp"
#include "Create/GL_Create_SSBOs.cpp"
#include "GL_Rasterizer_State_Manager.h"
#include "GL_Commands.h"

#include "Core/Application.h"
#include "Core/GraphicsContext.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace OpenGL::Renderer
{
	namespace {
		std::vector<GLuint> g_TextureHandles;
		GLuint g_EmptyVAO = 0;
	}
	void Init()
	{
		CreateFrameBuffers();
		CreateShaders();
		CreateSSBOs();

		OpenGLRasterizerState* geometryPass = OpenGL::RasterizerStateManager::CreateRasterizerState("GeometryPass");
		geometryPass->depthTestEnable = true;
		geometryPass->blendEnable = false;
		geometryPass->cullFaceEnable = true;
		geometryPass->depthMask = true;
		geometryPass->depthFunc = GL_GREATER;
	}

	void DrawFullScreenTriangle() {
		BindEmptyVAO();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void PresentFinalImage(OpenGLFrameBuffer& presentFBO) {
		OpenGLShader* shader = OpenGL::ResourceManager::GetShaderPtr("Present");
		if (!shader) {
			NGN_CORE_ERROR("Present shader not found");
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		glViewport(0, 0, NGN::Application::Get().GetWindow().GetWidth(), NGN::Application::Get().GetWindow().GetHeight());
		glDisable(GL_SCISSOR_TEST);

		OpenGLRasterizerState state;
		state.depthTestEnable = false;
		state.depthMask = false;
		state.cullFaceEnable = false;
		state.blendEnable = false;
		state.colorMask = true;

		OpenGL::RasterizerStateManager::ForceRasterizerState(state);

		Commands::BindShader("Present");
		Commands::BindTextureUnit(0, presentFBO.GetColorAttachmentSlotByName("Color"));
		DrawFullScreenTriangle();
	}

	void BindEmptyVAO() {
		if (g_EmptyVAO == 0) glGenVertexArrays(1, &g_EmptyVAO);
		glBindVertexArray(g_EmptyVAO);
	}

	void Shutdown() {}
}
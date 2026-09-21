#include "GL_Renderer.h"
//#include "GL_Backend.h"

#include "GL_Rasterizer_State_Manager.h"
#include "GL_Commands.h"
#include "Types/GL_IndirectBuffer.h"

#include "Core/Application.h"
#include "Core/GraphicsContext.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace OpenGL::Renderer
{
	namespace {
		std::vector<GLuint> g_TextureHandles;
		GLuint g_EmptyVAO = 0;
		IndirectBuffer g_IndirectBuffer;
	}
	void Init()
	{
		CreateFrameBuffers();
		CreateShaders();
		CreateSSBOs();

		OpenGLRasterizerState* geometryPass = OpenGL::RasterizerStateManager::CreateRasterizerState("GeometryPass");
		geometryPass->depthTestEnable = false;
		geometryPass->blendEnable = false;
		geometryPass->cullFaceEnable = false;
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

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		OpenGLRasterizerState state;
		state.depthTestEnable = false;
		state.depthMask = false;
		state.cullFaceEnable = false;
		state.blendEnable = false;
		state.colorMask = true;
		OpenGL::RasterizerStateManager::ForceRasterizerState(state);

		Commands::BindShader("Present");

		// Hack for now 
		Commands::BindTextureUnit(0, presentFBO.GetColorAttachmentHandleByName("BaseColorMetallic"));
		/*Commands::BindTextureUnit(0, presentFBO.GetColorAttachmentSlotByName("Color"));*/
		DrawFullScreenTriangle();
	}

	void BindEmptyVAO() {
		if (g_EmptyVAO == 0) glGenVertexArrays(1, &g_EmptyVAO);
		glBindVertexArray(g_EmptyVAO);
	}

	void SetViewport(OpenGLFrameBuffer* frameBuffer,const NGN::Camera* camera) {
		GLuint fbWidth = frameBuffer->GetWidth();
		GLuint fbHeight = frameBuffer->GetHeight();
		glm::vec2 pos = camera->GetPosition();
	}

	void Shutdown() {}

	void MultiDrawIndirect(const std::vector<DrawIndexedIndirectCommand>& commands) {
		if (commands.size()) {
			g_IndirectBuffer.Update(sizeof(DrawIndexedIndirectCommand) * commands.size(), commands.data());
			g_IndirectBuffer.Bind();

			glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0, (GLsizei)commands.size(), 0);
		}
	}

	IndirectBuffer& GetIndirectBuffer() { return g_IndirectBuffer; }
}
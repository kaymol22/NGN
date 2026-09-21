#include "Render/API/OpenGL/GL_Renderer.h"
#include "Render/API/OpenGL/GL_ResourceManager.h"
#include "Core/Application.h"
#include <glm/glm.hpp>

namespace OpenGL::Renderer
{
	void CreateFrameBuffers() {
		// TODO: Get some kind of user defined resolutions for buffers fed in here

		OpenGLFrameBuffer& gBuffer = OpenGL::ResourceManager::CreateFrameBuffer("gBuffer");
		gBuffer.Create(NGN::Application::Get().GetResolutions().GetGBufferRes());
		gBuffer.CreateAttachment("BaseColorMetallic", GL_RGBA8);
		gBuffer.CreateAttachment("NormalRoughness", GL_RGBA16F, GL_LINEAR, GL_LINEAR);
		gBuffer.CreateDepthAttachment(GL_DEPTH24_STENCIL8);

		OpenGLFrameBuffer& finalImageFBO = OpenGL::ResourceManager::CreateFrameBuffer("FinalImage");
		finalImageFBO.Create(NGN::Application::Get().GetResolutions().GetFinalRes());
		finalImageFBO.CreateAttachment("Color", GL_RGBA16F);

		OpenGLFrameBuffer& presentFBO = OpenGL::ResourceManager::CreateFrameBuffer("Present");
		presentFBO.Create(glm::ivec2(1280, 720));
		presentFBO.CreateAttachment("Color", GL_RGBA8, GL_NEAREST, GL_NEAREST);
	}
}
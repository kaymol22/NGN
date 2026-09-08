#include "Render/API/OpenGL/Types/GL_FrameBuffer.h"
#include "Render/API/OpenGL/GL_ResourceManager.h"
#include <glm/glm.hpp>

namespace OpenGL::Renderer
{
	static void CreateFrameBuffers() {
		// TODO: Get some kind of user defined resolutions for buffers fed in here

		OpenGLFrameBuffer& gBuffer = OpenGL::ResourceManager::CreateFrameBuffer("gBuffer");
		gBuffer.Create(glm::ivec2(1280, 720));
		gBuffer.CreateAttachment("BaseColor+Config", GL_RGBA8);
		gBuffer.CreateAttachment("Lighting", GL_RGBA16F, GL_LINEAR, GL_LINEAR);
		gBuffer.CreateDepthAttachment(GL_DEPTH24_STENCIL8);

		OpenGLFrameBuffer& finalImageFBO = OpenGL::ResourceManager::CreateFrameBuffer("Final");
		finalImageFBO.Create(glm::ivec2(1280, 720));
		finalImageFBO.CreateAttachment("Color", GL_RGBA16F);
	}
}
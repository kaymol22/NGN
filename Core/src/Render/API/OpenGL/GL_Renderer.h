#pragma once
#include "Render/API/OpenGL/Types/GL_FrameBuffer.h"
#include "Render/Camera.h"

namespace OpenGL::Renderer
{
	void Init();
	void DrawFullScreenTriangle();
	void BindEmptyVAO();
	void PresentFinalImage(OpenGLFrameBuffer& presentFBO);
	void SetViewport(OpenGLFrameBuffer* frameBuffer, const NGN::Camera* camera);
	void Shutdown();
}
#pragma once
#include "Render/API/OpenGL/Types/GL_FrameBuffer.h"

namespace OpenGL::Renderer
{
	void Init();
	void DrawFullScreenTriangle();
	void BindEmptyVAO();
	void PresentFinalImage(OpenGLFrameBuffer& presentFBO);
	void Shutdown();
	void OnWindowResize();
}
#pragma once

namespace OpenGL::Renderer
{
	void Init(int width, int height);
	void RenderScene();
	void Shutdown();
	void OnWindowResize();
}
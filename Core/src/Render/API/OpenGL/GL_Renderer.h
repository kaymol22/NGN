#pragma once
#include "common/GL_binding_indices.glsl"
#include "Render/DrawCommands.h"
#include "Render/API/OpenGL/Types/GL_FrameBuffer.h"
#include "Render/Camera.h"

namespace OpenGL::Renderer
{
	void Init();
	void Shutdown();

	void ClearRenderTargets();
	void RenderDeferred();

	void CreateFrameBuffers();
	void CreateShaders();
	void CreateSSBOs();
	// CreateTextureArrays();

	void UpdateSSBOs();

	void DrawFullScreenTriangle();
	void BindEmptyVAO();
	void PresentFinalImage(OpenGLFrameBuffer& presentFBO);
	void SetViewport(OpenGLFrameBuffer* frameBuffer, const NGN::Camera* camera);


	// Passes
	void GeometryPass();
	void TestPass();
	void DebugPass();


	void MultiDrawIndirect(const std::vector<DrawIndexedIndirectCommand>& commands);
}
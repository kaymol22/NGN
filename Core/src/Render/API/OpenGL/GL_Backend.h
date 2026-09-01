#pragma once
#include "Render/RenderBackend.h"
#include "GL_Commands.h"
#include "ResourceManagement/CPU/ResourceManager.h"
#include "ResourceManagement/CPU/Types/Texture.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace OpenGL
{
	class OpenGLBackend : public NGN::RenderBackend
	{
	public:
		OpenGLBackend() = default;
		~OpenGLBackend() { Shutdown(); }
		void Init(void* nativeWindowHandle) override;
		void Shutdown() override;
		void BeginFrame() override;
		void EndFrame() override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void AllocateTextureMemory(RS::Texture& texture);

		void UpdateBindlessTextures();
	private:
		GLFWwindow* m_WindowHandle;
	};
}
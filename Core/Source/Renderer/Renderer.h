#pragma once

// TODO: Abstract Core renderer class from GL/GLFW
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "RendererAPI.h"
#include <filesystem>

namespace Renderer
{

	struct Texture {
		GLuint Handle = 0;
		uint32_t Width = 0;
		uint32_t Height = 0;
	};

	struct Framebuffer {
		GLuint Handle = 0;
		Texture ColorAttachment;
	};

	// TODO: Change down the line to take camera as arg
	void BeginScene(const glm::mat4& viewProjection);
	void EndScene();


	// TODO: Separate to Renderer & Renderer2D 
	void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

	Texture CreateTexture(int width, int height);
	Texture LoadTexture(const std::filesystem::path& path);
	Framebuffer CreateFramebufferWithTexture(const Texture texture);
	bool AttachTextureToFramebuffer(Framebuffer& framebuffer, const Texture texture);
	void BlitFramebufferToSwapchain(const Framebuffer framebuffer);
}

namespace NGN
{
	class Renderer
	{
	public:
		static void Init(RendererAPIType api = RendererAPIType::OpenGL);
		static void Shutdown();

		void BeginScene(const glm::mat4& viewProjection);
		void EndScene();

	private:
		RendererAPIType s_API;
	};
}
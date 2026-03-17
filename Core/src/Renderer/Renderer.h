#pragma once

// TODO: Abstract Core renderer class from GL/GLFW
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "RenderCommand.h"
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
		static void Init();
		static void Shutdown();

		static void BeginScene(const glm::mat4& viewProjection);
		static void EndScene();

		static void OnWindowResize(uint32_t width, uint32_t height);

		// TODO: Research renderer submission & queues - shader, VAO's, transforms

		static RendererAPIType GetAPI() { return RendererAPI::GetAPI();  }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
}
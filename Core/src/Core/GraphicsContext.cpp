#include "GraphicsContext.h"
#include "Render/API/OpenGL/GL_Backend.h"
#include "Render/API/OpenGL/GL_Renderer.h"

namespace NGN
{
	void GraphicsContext::Init(void* nativeWindowHandle)
	{
		if (m_API == API::OPENGL)
		{
			m_Backend = CreateScope<OpenGL::OpenGLBackend>();
		}
		else if (m_API == API::VULKAN)
		{
			NGN_ASSERT(false, "Vulkan backend not supported yet");
			return;
		}
		else
		{
			NGN_CORE_INFO("API Undefined - could not initialize graphics backend");
			return;
		}
		m_Backend->Init(nativeWindowHandle);
		m_RenderGraph = CreateScope<RenderGraph>();
		m_RenderGraph->Build(); // Default pass registration
	}

	void GraphicsContext::BeginFrame(float lastFrameTime)
	{
		m_Backend->BeginFrame();
	}

	void GraphicsContext::Flush()
	{
		if (m_API == API::OPENGL)
		{
			OpenGL::Commands::Clear();
			OpenGL::Renderer::RenderScene();
		}
		else {
			NGN_CORE_INFO("GraphicsContext::Flush - unsupported graphics API, cannot render scene");
		}

	}

	void GraphicsContext::EndFrame()
	{
		m_Backend->EndFrame();
	}

	void GraphicsContext::OnWindowResize(uint32_t width, uint32_t height)
	{
		m_Backend->SetViewport(0, 0, width, height);
	}

	void GraphicsContext::SetAPI(API api)
	{
		m_API = api;
	}

	void GraphicsContext::Shutdown()
	{
		// Destroy backend object
		// Shutdown and clear out backend specific resource manager
		NGN_CORE_INFO("Cleaning up graphics context");
	}

	void GraphicsContext::SwitchAPI(API newAPI)
	{
	}
}
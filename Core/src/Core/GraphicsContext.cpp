#include "GraphicsContext.h"
#include "Render/FrameManager.h"
#include "Render/API/OpenGL/GL_Backend.h"
#include "Render/Renderer.h"

namespace NGN
{
	class Application;

	GraphicsContext::GraphicsContext(API api) : m_API(api) {}

	void GraphicsContext::Init(void* nativeWindowHandle)
	{
		if (m_API == API::OPENGL)
		{
			m_Backend = CreateScope<OpenGL::OpenGLBackend>();
			m_Backend->Init(nativeWindowHandle);
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
		Renderer::Init(m_API);
	}

	void GraphicsContext::BeginFrame()
	{
		m_Backend->BeginFrame();
		Renderer::FrameManager::BeginFrame();
	}

	void GraphicsContext::Flush()
	{
		Renderer::RenderScene();
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
		Renderer::Shutdown();
		m_Backend->Shutdown();
		m_Backend.reset();
	}

	void GraphicsContext::SwitchAPI(API newAPI)
	{
	}
}
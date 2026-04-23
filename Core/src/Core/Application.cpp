/*
	Owns main window, manages update loop, tracks layers
*/
#include "ngnpch.h"
#include "Core/Profile.h"
#include "Core/Timestep.h"
#include "Input/Input.h"

#include "Application.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Utils/PlatformUtils.h"

#include <glm/glm.hpp>

// Need for reverse iteration of layer stack on event dispatch - may remove later
#include <ranges>


namespace NGN {

	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
	{
		NGN_PROFILE_FUNCTION();
		NGN_CORE_ASSERT(!s_Application, "Application already exists!");
		s_Application = this;

		if (m_Specification.WindowSpec.Title.empty())
			m_Specification.WindowSpec.Title = m_Specification.Name;

		// Select renderer API from app spec
		{
			NGN_PROFILE_SCOPE("RendererAPI Init");
			RendererAPI::SetAPI(m_Specification.RendererAPI);
		}

		{
			NGN_PROFILE_SCOPE("Window Init");
			// Window Creation + Graphics Context initialization
			m_Window = Window::Create(m_Specification.WindowSpec);
			m_Window->SetEventCallback([this](Event& e) { this->RaiseEvent(e); });
		}

		// Initialise renderer (backend + rendercommand)
		Renderer::Init();
		// Initialise input system (platform specific)
		Input::Init();

		// Call resize event once at startup
		glm::vec2 size = glm::vec2(m_Window->GetWidth(), m_Window->GetHeight());
		Renderer::OnWindowResize(
			static_cast<uint32_t>(size.x),
			static_cast<uint32_t>(size.y)
		);
		
		m_ImGuiLayer = NGN::CreateRef<ImGuiLayer>();
		m_ImGuiLayer->OnAttach();
	}

	Application::~Application()
	{
		NGN_PROFILE_FUNCTION();

		/*m_Window::Shutdown();*/
		Renderer::Shutdown();
		Input::Shutdown();

		s_Application = nullptr;
	}

	void Application::Run()
	{
		m_Running = true;

		// Main App Loop
		while (m_Running)
		{
			Instrumentor::Get().ClearFrameResults();

			NGN_PROFILE_SCOPE("Run Loop");

			float currentTime = Time::GetTime();
			m_Timestep = currentTime - m_LastFrameTime;
			m_LastFrameTime = currentTime;

			glm::vec2 framebufferSize = Application::GetFramebufferSize();

			/*========== Updates =============*/
			{
				NGN_PROFILE_SCOPE("LayerStack OnUpdate");
				for (const std::unique_ptr<Layer>& layer : m_LayerStack)
					layer->OnUpdate(m_Timestep);
			}

			/*========== Rendering =============*/
			NGN::RenderCommand::SetViewport(0, 0, framebufferSize.x, framebufferSize.y);
			Renderer::BeginFrame();

			// scene->RenderScene() called in layer OnUpdate,
			// Data submitted to renderer - allows for batching and sorting of draw calls, etc. in 2D renderer
			Renderer::Flush();

			/*========== ImGuiRendering =============*/
			m_ImGuiLayer->Begin();
			{
				NGN_PROFILE_SCOPE("LayerStack OnImGuiRender");
				for (const std::unique_ptr<Layer>& layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			Renderer::EndFrame();
			m_Window->OnUpdate();

			for (auto& transition : m_PendingTransitions)
				transition();
			m_PendingTransitions.clear();
		}
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	void Application::RaiseEvent(Event& event)
	{
		NGN_PROFILE_FUNCTION();
		// Application level events handled first
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(NGN_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(NGN_BIND_EVENT_FN(Application::OnWindowResize));

		// Go to layer stack if not handled
		for (auto& layer : std::views::reverse(m_LayerStack))
		{
			layer->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Stop();
		e.Handled = true;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		e.Handled = true;
		return false;
	}

	glm::vec2 Application::GetFramebufferSize() const
	{
		return glm::vec2(m_Window->GetWidth(), m_Window->GetHeight());
	}

	Application& Application::Get()
	{
		NGN_CORE_ASSERT(s_Application);
		return *s_Application;
	}
}
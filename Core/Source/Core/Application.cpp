/*
	Owns main window, manages update loop, tracks layers
*/
#include "Core/ngnpch.h"
#include "Core/Profile.h"

#include "Application.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Remove later
#include <imgui.h>

#include <assert.h>
#include <ranges>


namespace NGN {

	static Application* s_Application = nullptr;

	static void GLFWErrorCallback(int error, const char* description)
	{
		std::cerr << "[GLFW Error]: " << description << std::endl;
	}

	Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
	{
		NGN_CORE_ASSERT(!s_Application, "Application already exists!");
		s_Application = this;

		glfwSetErrorCallback(GLFWErrorCallback);
		glfwInit();

		if (m_Specification.WindowSpec.Title.empty())
			m_Specification.WindowSpec.Title = m_Specification.Name;

		m_Specification.WindowSpec.EventCallback = [this](Event& event) { RaiseEvent(event); };

		// Window Creation
		m_Window = NGN::CreateScope<Window>(m_Specification.WindowSpec);
		m_Window->Create();

		// Select renderer API from app spec
		RendererAPI::SetAPI(m_Specification.RendererAPI);

		// Initialise renderer (backend + rendercommand)
		Renderer::Init();
		// Call resize event once at startup
		glm::vec2 size = m_Window->GetFramebufferSize();
		Renderer::OnWindowResize(
			static_cast<uint32_t>(size.x),
			static_cast<uint32_t>(size.y)
		);
		
		m_ImGuiLayer = NGN::CreateRef<ImGuiLayer>();
		m_ImGuiLayer->OnAttach();
	}

	Application::~Application()
	{
		m_Window->Destroy();
		glfwTerminate();

		s_Application = nullptr;
	}

	void Application::Run()
	{
		m_Running = true;

		float lastTime = GetTime();

		// Main App Loop
		while (m_Running)
		{
			Instrumentor::Get().ClearFrameResults();

			NGN_PROFILE_FUNCTION("Run Loop");
			glfwPollEvents();

			if (m_Window->ShouldClose())
			{
				Stop();
				break;
			}

			float currentTime = GetTime();
			float delta = currentTime - lastTime;
			lastTime = currentTime;

			m_Timestep = Timestep(glm::clamp(delta, 0.001f, 0.1f));
			glm::vec2 framebufferSize = Application::GetFramebufferSize();

			/*========== Updates =============*/
			// TODO: Look into fixed vs variable update - future expansion for suspending/pausing layers
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnUpdate(m_Timestep);

			/*========== Rendering =============*/
			glm::vec4 clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
			NGN::RenderCommand::SetViewport(0, 0, framebufferSize.x, framebufferSize.y);
			NGN::RenderCommand::SetClearColor(clearColor);
			NGN::RenderCommand::Clear();

			// World Space Render
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
			{
				if (layer->HasFlag(LayerFlags::WorldSpace))
					layer->OnRender();
			}

			// ScreenSpace/Overlay Render
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
			{
				if (layer->HasFlag(LayerFlags::ScreenSpace))
					layer->OnRender();
			}

			// Debug/Profiling Render
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
			{
				if (layer->HasFlag(LayerFlags::Debug))
					layer->OnRender();
			}

			/*========== ImGuiRendering =============*/
			m_ImGuiLayer->Begin();

			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
			{
				if (layer->HasFlag(LayerFlags::UsesImGui))
					layer->OnImGuiRender();
			}

			m_ImGuiLayer->End();

			m_Window->Update();

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
		for (auto& layer : std::views::reverse(m_LayerStack))
		{
			layer->OnEvent(event);
			if (event.Handled)
				break;
		}
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

		return false;
	}

	glm::vec2 Application::GetFramebufferSize() const
	{
		return m_Window->GetFramebufferSize();
	}

	Application& Application::Get()
	{
		assert(s_Application);
		return *s_Application;
	}

	float Application::GetTime()
	{
		return (float)glfwGetTime();
	}
}
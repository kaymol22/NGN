/*
	Owns main window, manages update loop, tracks layers
*/
#include "ngnpch.h"
#include "Core/Profile.h"
#include "Core/Timestep.h"
#include "Input/Input.h"

#include "Application.h"
#include "Renderer/Backend/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Utils/PlatformUtils.h"

#include <glm/glm.hpp>

// Need for reverse iteration of layer stack on event dispatch - may remove later
#include <ranges>

namespace NGN {

	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& spec) : m_Specification(spec)
	{
		NGN_PROFILE_SCOPE("App Init");
		NGN_CORE_ASSERT(!s_Application, "Application already exists!");
		s_Application = this;

		if (spec.WindowSpec.Title.empty())
			m_Specification.WindowSpec.Title = spec.Name;

		m_GraphicsContext = CreateScope<GraphicsContext>();
		m_GraphicsContext->SetAPI(spec.APIspec);

		m_Window = Window::Create(spec.APIspec, spec.WindowSpec);
		m_Window->SetEventCallback([this](Event& e) { this->RaiseEvent(e); });

		m_GraphicsContext->Init(m_Window->GetNativeWindow());
		m_GraphicsContext->OnWindowResize(m_Window->GetWidth(), m_Window->GetHeight());

		m_ResourceManager = CreateScope<RS::ResourceManager>();
		Input::Init();
		// Asset compiling & loading

		m_ImGuiLayer = NGN::CreateRef<ImGuiLayer>();
		m_ImGuiLayer->OnAttach();
	}

	Application::~Application()
	{
		NGN_PROFILE_FUNCTION();
		m_GraphicsContext->Shutdown();
		// Window::Shutdown() called in window destructor
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

			m_Window->BeginFrame();
			m_GraphicsContext->BeginFrame(m_LastFrameTime);

			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnUpdate(m_Timestep);

			m_GraphicsContext->Flush();

			m_ImGuiLayer->Begin();
			{
				NGN_PROFILE_SCOPE("LayerStack OnImGuiRender");
				for (const std::unique_ptr<Layer>& layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_GraphicsContext->EndFrame();
			m_Window->EndFrame(m_GraphicsContext->GetAPI());

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
		/*Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());*/
		m_GraphicsContext->OnWindowResize(e.GetWidth(), e.GetHeight());

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

	Scene* Application::CreateScene(const std::string& name, bool setActive)
	{
		auto scene = NGN::CreateScope<Scene>();
		Scene* scenePtr = scene.get();
		GetSceneManager().RegisterScene(name, std::move(scene));
		if (setActive)
			GetSceneManager().SetActiveScene(name);
		return scenePtr;
	}
}
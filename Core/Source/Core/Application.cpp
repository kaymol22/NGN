/*
	Owns main window, manages update loop, tracks layers
*/
#include "Core/ngnpch.h"

#include "Application.h"
#include "Renderer/GLUtils.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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
		s_Application = this;

		glfwSetErrorCallback(GLFWErrorCallback);
		glfwInit();

		if (m_Specification.WindowSpec.Title.empty())
			m_Specification.WindowSpec.Title = m_Specification.Name;

		m_Specification.WindowSpec.EventCallback = [this](Event& event) { RaiseEvent(event); };

		m_Window = NGN::CreateScope<Window>(m_Specification.WindowSpec);
		m_Window->Create();

		Renderer::Utils::InitOpenGLDebugMessageCallback();
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
			glfwPollEvents();

			if (m_Window->ShouldClose())
			{
				Stop();
				break;
			}

			float currentTime = GetTime();
			float timeStep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
			lastTime = currentTime;

			// Main Layer update here
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnUpdate(timeStep);

			// Keep Render and update separate - manual management of render thread
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnRender();


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
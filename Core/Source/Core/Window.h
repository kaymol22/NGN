#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Events/Event.h"

namespace NGN {

	struct WindowSpecification
	{
		std::string Title;
		uint32_t Width = 1280;
		uint32_t Height = 720;
		bool IsResizeable = true;
		bool VSync = true;

		using EventCallbackFn = std::function<void(Event&)>;
		EventCallbackFn EventCallback;
	};

	class Window
	{
	public:
		Window(const WindowSpecification& specification = WindowSpecification());
		~Window();

		void Create();
		void Destroy();

		void Update();

		void RaiseEvent(Event& event);

		glm::vec2 GetFramebufferSize() const;
		glm::vec2 GetMousePos() const;

		bool ShouldClose() const;

		void SetVSync(bool enabled)
		{
			glfwSwapInterval(enabled ? 1 : 0);
			m_Specification.VSync = enabled;
		}

		bool IsVSync() const { return m_Specification.VSync; }

		GLFWwindow* GetHandle() const { return m_Handle; }
	private:
		WindowSpecification m_Specification;

		GLFWwindow* m_Handle = nullptr;

	};

}
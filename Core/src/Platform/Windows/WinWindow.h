#pragma once

#include "Renderer/GraphicsContext.h"
#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace NGN
{
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowSpecification& spec);
		virtual ~WinWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowSpecification& spec);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width;
			uint32_t Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
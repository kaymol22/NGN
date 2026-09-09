#pragma once

#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace NGN
{
	class WinWindow : public Window
	{
	public:
		WinWindow(const API api, const WindowSpecification& spec);
		virtual ~WinWindow();

		void BeginFrame() override;
		void EndFrame(API api) override;

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
		API m_API = API::UNDEFINED;

		struct WindowData
		{
			std::string Title;
			uint32_t Width = 0;
			uint32_t Height = 0;
			bool VSync = true;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
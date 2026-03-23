#pragma once

#include "Events/Event.h"

namespace NGN {

	struct WindowSpecification
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool IsResizeable = true;
		bool VSync = true;

		WindowSpecification(const std::string& title = "NGN Engine",
			uint32_t width = 1600,
			uint32_t height = 900,
			bool isResizeable = true,
			bool vSync = true)
			: Title(title), Width(width), Height(height), IsResizeable(isResizeable), VSync(vSync)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		// Platform-specific window handle for graphics context, etc.
		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowSpecification& spec = WindowSpecification());
	};

}
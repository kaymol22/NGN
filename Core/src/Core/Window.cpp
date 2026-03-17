#include "ngnpch.h"

#include <glad/gl.h>

#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#ifdef NGN_PLATFORM_WINDOWS
	#include "Platform/Windows/WinWindow.h"
#endif

namespace NGN {

	Scope<Window> Window::Create(const WindowSpecification& specification)
	{
		#ifdef NGN_PLATFORM_WINDOWS
			return CreateScope<WinWindow>(specification);
		#else
			NGN_CORE_ASSERT(false, "Unknown platform, cannot create window");
			return nullptr;
		#endif
	}
}
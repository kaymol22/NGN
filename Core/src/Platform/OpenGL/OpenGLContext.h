#pragma once
#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace NGN
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(void* windowHandle);
		virtual ~OpenGLContext() = default;

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		void* m_WindowHandle;
	};
}
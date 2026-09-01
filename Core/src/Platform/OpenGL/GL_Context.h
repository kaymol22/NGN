#pragma once
#include "Renderer/Backend/GraphicsContext.h"

struct GLFWwindow;

namespace NGN
{
	class OpenGLContext : public GraphicsContextOld
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
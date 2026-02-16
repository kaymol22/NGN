#pragma once

namespace NGN
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> CreateGraphicsContext(void* window);
	};
}
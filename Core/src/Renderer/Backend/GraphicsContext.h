#pragma once

namespace NGN
{
	class GraphicsContextOld
	{
	public:
		virtual ~GraphicsContextOld() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContextOld> CreateGraphicsContext(void* window);
	};
}
#pragma once

#include "RendererAPI.h"

#include <glm/glm.hpp>

namespace NGN
{
	class RenderCommand
	{
	public:
		static void Init();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}
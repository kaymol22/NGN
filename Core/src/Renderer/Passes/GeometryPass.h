#pragma once

#include "Renderer/RenderPass.h"

namespace NGN
{
	class GeometryPass : public RenderPass
	{
	public:
		virtual ~GeometryPass() = default;

		static Scope<GeometryPass> Create();
	};
}
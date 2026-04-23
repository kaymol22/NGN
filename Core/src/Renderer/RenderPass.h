#pragma once

#include "Framebuffer.h"

namespace NGN
{
	enum class PassType
	{
		Geometry3D, // Pass into g-buffer
		Lighting, // Deferred lighting
		Sprite2D,
		UI,
		Compose, // Final
		Debug
	};

	struct RenderPass
	{
		PassType type;
		Ref<Framebuffer> targetFramebuffer;
		std::function<void()> executeFunction; // Keep what to execute flexible
	};
}
#pragma once

#include "System.h"

namespace NGN
{
	class SpriteRenderSystem : public System
	{
		~SpriteRenderSystem() = default;
		void OnCreate(Scene& scene) override;
		void OnUpdate(Scene& scene, Timestep ts) override;
	};
}
#pragma once
#include "System.h"

namespace NGN
{
	class RenderSystem : public System
	{
	public:
		~RenderSystem() = default;
		void OnCreate(Scene& scene) override;
		void OnUpdate(Scene& scene, Timestep ts) override;
	};
}
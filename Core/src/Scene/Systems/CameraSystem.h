#pragma once
#include "System.h"

namespace NGN
{
	class CameraSystem : public System
	{
	public:
		~CameraSystem() = default;
		void OnCreate(Scene& scene) override;
		void OnUpdate(Scene& scene, Timestep ts) override;
	};
}
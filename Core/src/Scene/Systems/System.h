#pragma once

#include "Core/Timestep.h"
#include "Render/Camera.h"

#include "Scene/SceneCamera.h"

namespace NGN
{
	class Scene;

	class System
	{
	public:
		virtual ~System() = default;
		virtual void OnCreate(Scene& scene) {}
		virtual void OnUpdate(Scene& scene, Timestep ts) {}
	};
}

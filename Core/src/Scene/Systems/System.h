#pragma once

#include "Core/Timestep.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Resources/Camera.h"
#include "Renderer/Resources/OrthographicCamera.h"
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

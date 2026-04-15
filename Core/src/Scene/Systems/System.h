#pragma once

#include "Core/Timestep.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Camera.h"
#include "Renderer/OrthographicCamera.h"
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
		virtual void OnRender(Scene& scene) {}
		virtual void OnRender(Scene& scene, const OrthographicCamera& camera) {}
		virtual void OnRender(Scene& scene, const Camera& camera) {}
		virtual void OnRender(Scene& scene, const SceneCamera& camera) {}
	};
}
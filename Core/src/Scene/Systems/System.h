#pragma once

#include "Core/Timestep.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/OrthographicCamera.h"

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
	};
}
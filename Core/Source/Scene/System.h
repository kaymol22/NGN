#pragma once

#include "Renderer/OrthographicCamera.h"

namespace NGN
{
	class Scene;

	class System
	{
	public:
		virtual ~System() = default;

		virtual void OnCreate(Scene& scene) {}
		virtual void OnUpdate(Scene& scene, float dt) {}
		virtual void OnRender(Scene& scene) {}
		/*virtual void OnRender(Scene& scene, const OrthographicCamera& camera) {}*/
	};
}
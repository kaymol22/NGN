#pragma once

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
	};
}
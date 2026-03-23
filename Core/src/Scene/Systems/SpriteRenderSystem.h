#pragma once

#include "Scene/Scene.h"
#include "System.h"
#include "Scene/Components.h"
#include "Renderer/OrthographicCamera.h"

namespace NGN
{
	class SpriteRenderSystem : public System
	{
	public:
		void OnCreate(Scene& scene) override;
		void OnUpdate(Scene& scene, Timestep ts) override;
		void OnRender(Scene& scene, const OrthographicCamera& camera);
	};
}
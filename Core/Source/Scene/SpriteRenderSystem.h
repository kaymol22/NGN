#pragma once

#include "Scene.h"
#include "System.h"
#include "Components.h"
#include "Renderer/OrthographicCamera.h"

namespace NGN
{
	class SpriteRenderSystem : public System
	{
	public:
		void OnCreate(Scene& scene) override;
		void OnRender(Scene& scene) override;
	};
}
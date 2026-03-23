#pragma once

#include "NGN.h"

#include "Renderer/Renderer2D.h"

#include <iostream>
//TEMP: debug
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class GameLayer : public NGN::Layer
{
public:
	GameLayer();
	
	virtual void OnAttach() override;

	virtual void OnDetach() override;

	virtual void OnUpdate(NGN::Timestep ts) override;

	virtual void OnEvent(NGN::Event& e) override;
	
private:
	std::unique_ptr<NGN::Scene> m_Scene;
	NGN::OrthographicCameraController m_CameraController;

	NGN::Ref<NGN::Texture2D> m_CheckerBoardTexture;
};
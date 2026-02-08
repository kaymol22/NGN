#pragma once

#include <glm/glm.hpp>

#include <stdint.h>

#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"

#include "Core/Layer.h"

class AppLayer : public NGN::Layer
{
public:
	AppLayer();

	virtual ~AppLayer();

	virtual void OnEvent(NGN::Event& event) override;

	virtual void OnUpdate(NGN::Timestep ts) override;
	virtual void OnRender() override;
private:
	bool OnMouseButtonPressed(NGN::MouseButtonPressedEvent& event);
	bool OnMouseMoved(NGN::MouseMovedEvent& event);
	bool OnWindowClosed(NGN::WindowCloseEvent& event);
private:
	uint32_t m_Shader = 0;
	uint32_t m_VertexArray = 0;
	uint32_t m_VertexBuffer = 0;

	float m_Time = 0.0f;

	glm::vec2 m_MousePosition{ 0.0f };
	glm::vec2 m_FlamePosition{ 0.0f };
};
#pragma once

#include <stdint.h>

#include "Core/Layer.h"

#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include "Renderer/Renderer.h"

class OverlayLayer : public NGN::Layer
{
public:
	OverlayLayer();
	virtual ~OverlayLayer();

	virtual void OnEvent(NGN::Event& event) override;

	void OnUpdate(float timeStep) override;
	void OnRender() override;
private:
	bool IsButtonHovered() const;

	bool OnMouseButtonPressed(NGN::MouseButtonPressedEvent& event);

private:
	uint32_t m_Shader = 0;
	uint32_t m_VertexArray = 0;
	uint32_t m_VertexBuffer = 0;
	uint32_t m_IndexBuffer = 0;
	Renderer::Texture m_Texture;

	bool m_IsHovered = false;
	bool m_Pressed = true;
};
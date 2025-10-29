#pragma once

#include <stdint.h>

#include "Core/Layer.h"
#include "Renderer/Renderer.h"

class OverlayLayer : public Core::Layer
{
public:
	OverlayLayer();
	virtual ~OverlayLayer();

	void OnUpdate(float timeStep) override;
	void OnRender() override;
private:
	uint32_t m_Shader = 0;
	uint32_t m_VertexArray = 0;
	uint32_t m_VertexBuffer = 0;
	uint32_t m_IndexBuffer = 0;
	Renderer::Texture m_Texture;

	bool m_IsHovered = false;
	bool m_Pressed = true;
};
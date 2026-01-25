#pragma once

#include <stdint.h>

#include "Core/Layer.h"

#include "Events/KeyEvent.h"

class VoidLayer : public NGN::Layer
{
public:
	VoidLayer() = default;
	virtual ~VoidLayer() = default;

	void OnUpdate(NGN::Timestep ts) override;
	void OnRender() override;
private:
	uint32_t m_Shader = 0;
	uint32_t m_VertexArray = 0;
	uint32_t m_VertexBuffer = 0;

	float m_Time = 0.0f;
};
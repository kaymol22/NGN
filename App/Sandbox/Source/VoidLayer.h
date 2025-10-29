#pragma once

#include <stdint.h>

#include "Core/Layer.h"

class VoidLayer : public Core::Layer
{
public:
	VoidLayer() = default;
	virtual ~VoidLayer() = default;

	void OnUpdate(float timeStep) override;
	void OnRender() override;
private:
	uint32_t m_Shader = 0;
	uint32_t m_VertexArray = 0;
	uint32_t m_VertexBuffer = 0;

	float m_Time = 0.0f;
};
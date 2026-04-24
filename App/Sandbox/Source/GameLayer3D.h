#pragma once
#include "NGN.h"

class GameLayer3D : public NGN::Layer
{
public:
	GameLayer3D();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(NGN::Timestep ts) override;
	virtual void OnEvent(NGN::Event& event) override;
};
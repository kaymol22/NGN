#pragma once
#include "NGN.h"

class BackendTestLayer : public NGN::Layer
{
public:
	BackendTestLayer();

	void OnAttach() override;
	void OnUpdate(NGN::Timestep ts) override;
	void OnEvent(NGN::Event& e) override;
private:
	
};
#pragma once
#include "NGN.h"

namespace NGN
{
	class GameLayer3D : public Layer
	{
	public:
		GameLayer3D();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;
	};
}
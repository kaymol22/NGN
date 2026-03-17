#pragma once

#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

namespace NGN
{
	namespace WindowsInput
	{
		void CreateInput();
		void DestroyInput();

		// Implementation funcs
		bool IsKeyPressedImpl(KeyCode key);
		bool IsMouseButtonPressedImpl(MouseCode button);
		float GetMouseXImpl();
		float GetMouseYImpl();
		std::pair<float, float> GetMousePositionImpl();
	};
}
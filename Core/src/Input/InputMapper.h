#pragma once
#include "InputState.h"
#include "InputBindings.h"

namespace NGN
{
	class InputMapper
	{
	public:
		virtual ~InputMapper() = default;
		virtual void Update(InputState& state, const InputBindings& bindings) = 0;
	};

	class KeyboardMouseInputMapper : public InputMapper
	{
	public:
		KeyboardMouseInputMapper() = default;
		void Update(InputState& state, const InputBindings& bindings) override;

	private:
		glm::vec2 m_LastMousePos { 0.0f, 0.0f };
		bool m_FirstMouseMove = true;
	};

	// TODO: Implement gamepad input after Input class support
	/*class GamepadInputMapper : public InputMapper
	{
		
	};*/
}
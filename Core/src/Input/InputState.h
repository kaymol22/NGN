#pragma once
#include <glm/glm.hpp>
#include <bitset>

#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

namespace NGN
{
	enum class ActionType
	{
		MoveForward,
		MoveBackward,
		MoveLeft,
		MoveRight,
		Jump,
		Sprint,
		Crouch,
		Interact,

		Look, // Mouse look input - special case

		Count // Use for sizing bitset
	};
	
	// For mapping of an action to one or more input codes (keyboard keys, mouse buttons, etc.)
	struct InputBinding
	{
		KeyCode PrimaryKey = Key::None;
		KeyCode SecondaryKey = Key::None;
		MouseCode MouseButton = Mouse::ButtonNone; // Default to invalid mouse button

		InputBinding() = default;
		InputBinding(KeyCode primary) : PrimaryKey(primary) {}
		InputBinding(KeyCode primary, KeyCode secondary) : PrimaryKey(primary), SecondaryKey(secondary) {}
		/*InputBinding(MouseCode mouseButton) : MouseButton(mouseButton) {}*/
	};

	struct InputState
	{
		// Normalized movement input vector (x: left/right, y: forward/backward)
		glm::vec2 MovementInput{ 0.0f, 0.0f };
		// Raw mouse delta for look input (x: yaw, y: pitch)
		glm::vec2 LookDelta { 0.0f, 0.0f };

		bool IsJumping = false;
		bool IsSprinting = false;
		bool IsCrouching = false;
		bool IsInteracting = false;

		std::bitset<16> ActionFlags; // Leave room for additional actions

		void Reset()
		{
			MovementInput = { 0.0f, 0.0f };
			LookDelta = { 0.0f, 0.0f }; // pure mouse delta
			IsJumping = false;
			IsSprinting = false;
			IsCrouching = false;
			IsInteracting = false;
			ActionFlags.reset();
		}
	};
}
#include "ngnpch.h"
#include "InputMapper.h"
#include "Input.h"

namespace NGN
{
	void KeyboardMouseInputMapper::Update(InputState& state, const InputBindings& bindings)
	{
		state.Reset();

		// Normalized movement input check
		glm::vec2 moveDir = { 0.0f, 0.0f };

		if (bindings.ActionPressed(ActionType::MoveForward))
			moveDir.y += 1.0f;
		if (bindings.ActionPressed(ActionType::MoveBackward))
			moveDir.y -= 1.0f;
		if (bindings.ActionPressed(ActionType::MoveLeft))
			moveDir.x -= 1.0f;
		if (bindings.ActionPressed(ActionType::MoveRight))
			moveDir.x += 1.0f;

		if (glm::length(moveDir) > 0.0f)
			moveDir = glm::normalize(moveDir);

		state.IsJumping = bindings.ActionPressed(ActionType::Jump);
		state.IsSprinting = bindings.ActionPressed(ActionType::Sprint);
		state.IsCrouching = bindings.ActionPressed(ActionType::Crouch);
		state.IsInteracting = bindings.ActionPressed(ActionType::Interact);

		if (bindings.ActionPressed(ActionType::Look))
		{
			glm::vec2 currentMousePos = Input::GetMousePositionVec2();

			if (m_FirstMouseMove)
			{
				m_LastMousePos = currentMousePos;
				m_FirstMouseMove = false;
			}
			else
			{
				state.LookDelta = currentMousePos - m_LastMousePos;
				m_LastMousePos = currentMousePos;
			}
		}
		else
		{
			m_FirstMouseMove = true; // Reset on release to prevent jump on next press
		}
	}
}
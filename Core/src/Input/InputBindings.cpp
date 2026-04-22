#include "ngnpch.h"
#include "InputBindings.h"
#include "Input.h"

namespace NGN
{
	InputBindings::InputBindings()
	{
		SetDefaults();
	}

	void InputBindings::SetDefaults()
	{
		m_Bindings[(size_t)ActionType::MoveForward] = InputBinding(Key::W);
		m_Bindings[(size_t)ActionType::MoveBackward] = InputBinding(Key::S);
		m_Bindings[(size_t)ActionType::MoveLeft] = InputBinding(Key::A);
		m_Bindings[(size_t)ActionType::MoveRight] = InputBinding(Key::D);
		m_Bindings[(size_t)ActionType::Jump] = InputBinding(Key::Space);
		m_Bindings[(size_t)ActionType::Sprint] = InputBinding(Key::LeftShift);
		m_Bindings[(size_t)ActionType::Crouch] = InputBinding(Key::LeftControl);
		m_Bindings[(size_t)ActionType::Interact] = InputBinding(Key::E);
		m_Bindings[(size_t)ActionType::Look] = InputBinding(Mouse::ButtonNone);
	}

	void InputBindings::SetBinding(ActionType action, const InputBinding& binding)
	{
		m_Bindings[(size_t)action] = binding;
		// TODO: Add event or callback for notifying/saving changes to bindings
	}

	const InputBinding& InputBindings::GetBinding(ActionType action) const
	{
		return m_Bindings[(size_t)action];
	}

	bool InputBindings::ActionPressed(ActionType action) const
	{
		const auto& binding = GetBinding(action);

		if (binding.PrimaryKey != Key::None && Input::IsKeyPressed(binding.PrimaryKey))
			return true;

		if (binding.SecondaryKey != Key::None && Input::IsKeyPressed(binding.SecondaryKey))
			return true;

		if (binding.MouseButton != Mouse::ButtonNone && Input::IsMouseButtonPressed(binding.MouseButton))
			return true;

		return false;
	}
}
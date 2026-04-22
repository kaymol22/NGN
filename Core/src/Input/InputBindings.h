#pragma once

#include "InputState.h"

namespace NGN
{
	class InputBindings
	{
	public:
		InputBindings();

		void SetBinding(ActionType action, const InputBinding& binding);
		const InputBinding& GetBinding(ActionType action) const;

		bool ActionPressed(ActionType action) const;

		void SetDefaults();
		void ResetAllBindings();

		const std::array<InputBinding, (size_t)ActionType::Count>& GetAllBindings() const { return m_Bindings; }
	
	private:
		std::array<InputBinding, (size_t)ActionType::Count> m_Bindings;
	};
}
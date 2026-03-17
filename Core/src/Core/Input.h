#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace NGN
{
	// Forward declare platform specific implementations - access to private members of Input class
	namespace WindowsInput 
	{
		void CreateInput();
		void DestroyInput();
	}

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		float GetMouseX();
		float GetMouseY();
		static std::pair<float, float> GetMousePosition();

	private:
		// Platform-specific implementation set at runtime
		static bool (*s_IsKeyPressed)(KeyCode);
		static bool (*s_IsMouseButtonPressed)(MouseCode);
		static float (*s_GetMouseX)();
		static float (*s_GetMouseY)();
		static std::pair<float, float>(*s_GetMousePosition)();

		friend class Application;

		friend void WindowsInput::CreateInput();
		friend void WindowsInput::DestroyInput();
		
		static void Init();
		static void Shutdown();
	};
}
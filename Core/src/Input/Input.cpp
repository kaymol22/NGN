#include "ngnpch.h"
#include "Input.h"
#include "Core/Application.h"

#ifdef NGN_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace NGN
{
	bool (*Input::s_IsKeyPressed)(KeyCode) = nullptr;
	bool (*Input::s_IsMouseButtonPressed)(MouseCode) = nullptr;
	float (*Input::s_GetMouseX)() = nullptr;
	float (*Input::s_GetMouseY)() = nullptr;
	std::pair<float, float>(*Input::s_GetMousePosition)() = nullptr;

	void Input::Init()
	{
		// Bind Platform Implementation
		#ifdef NGN_PLATFORM_WINDOWS
			WindowsInput::CreateInput();
		#endif
	}

	void Input::Shutdown()
	{
		#ifdef NGN_PLATFORM_WINDOWS
			WindowsInput::DestroyInput();
		#endif
	}

	bool Input::IsKeyPressed(KeyCode key)
	{
		return s_IsKeyPressed ? s_IsKeyPressed(key) : false;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		return s_IsMouseButtonPressed ? s_IsMouseButtonPressed(button) : false;
	}

	float Input::GetMouseX()
	{
		return s_GetMouseX ? s_GetMouseX() : 0.0f;
	}

	float Input::GetMouseY()
	{
		return s_GetMouseY ? s_GetMouseY() : 0.0f;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		return s_GetMousePosition ? s_GetMousePosition() : std::make_pair(0.0f, 0.0f);
	}

	glm::vec2 Input::GetMousePositionVec2()
	{
		auto [x, y] = GetMousePosition();
		return { x, y };
	}
}
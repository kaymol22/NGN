#include "ngnpch.h"
#include "Input/Input.h"
#include "WindowsInput.h"
#include "WinWindow.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace NGN::WindowsInput
{
	void CreateInput()
	{
		Input::s_IsKeyPressed = IsKeyPressedImpl;
		Input::s_IsMouseButtonPressed = IsMouseButtonPressedImpl;
		Input::s_GetMouseX = GetMouseXImpl;
		Input::s_GetMouseY = GetMouseYImpl;
		Input::s_GetMousePosition = GetMousePositionImpl;
	}

	void DestroyInput()
	{
		Input::s_IsKeyPressed = nullptr;
		Input::s_IsMouseButtonPressed = nullptr;
		Input::s_GetMouseX = nullptr;
		Input::s_GetMouseY = nullptr;
		Input::s_GetMousePosition = nullptr;
	}

	bool IsKeyPressedImpl(const KeyCode key)
	{
		if (key == Key::None)
			return false;
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, static_cast<int>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool IsMouseButtonPressedImpl(const MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, static_cast<int>(button));
		return state == GLFW_PRESS;
	}

	float GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

	std::pair<float, float> GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}
}
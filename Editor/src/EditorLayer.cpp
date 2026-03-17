#include "EditorLayer.h"

namespace NGN
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{

	}

	void EditorLayer::OnAttach()
	{
		NGN_PROFILE_FUNCTION();

		// Load & create textures for editor resources here
	}

	void EditorLayer::OnDetach()
	{
		NGN_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		NGN_PROFILE_FUNCTION();
		if (ViewportFocused)
		{
			//m_CameraController.OnUpdate(ts);
		}
		if (m_ActiveScene)
			m_ActiveScene->OnUpdate(ts);
	}

	void EditorLayer::OnImGuiRender()
	{
		NGN_PROFILE_FUNCTION();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(NGN_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(NGN_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
			case Key::S:
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}
				break;
		}
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}

	void EditorLayer::SaveScene()
	{
		NGN_CORE_INFO("SaveScene");
	}

	void EditorLayer::SaveSceneAs()
	{
		NGN_CORE_INFO("SaveSceneAs");
	}
}
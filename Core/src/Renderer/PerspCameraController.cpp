#include "ngnpch.h"
#include "PerspCameraController.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace NGN
{
	PerspectiveCameraController::PerspectiveCameraController(const ControllerSettings& settings)
		: m_Camera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f)
	{
		m_Camera.SetMinPitch(m_Settings.MinPitch);
		m_Camera.SetMaxPitch(m_Settings.MaxPitch);
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		NGN_PROFILE_FUNCTION();

		UpdateMovement(ts);
		/*UpdateRotation();*/
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		NGN_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(NGN_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(NGN_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(NGN_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	void PerspectiveCameraController::UpdateMovement(Timestep ts)
	{
		m_MoveDirection = glm::vec3(0.0f);

		// TODO: Refactor to use bitset or something - configurable key bindings
		m_IsSprinting = Input::IsKeyPressed(Key::LeftShift);

		if(Input::IsKeyPressed(Key::W))
			m_MoveDirection += glm::vec3(0.0f, 0.0f, 1.0f);
		if (Input::IsKeyPressed(Key::S))
			m_MoveDirection += glm::vec3(0.0f, 0.0f, -1.0f);
		if (Input::IsKeyPressed(Key::D))
			m_MoveDirection += glm::vec3(1.0f, 0.0f, 0.0f);
		if (Input::IsKeyPressed(Key::A))
			m_MoveDirection += glm::vec3(-1.0f, 0.0f, 0.0f);

		if (Input::IsKeyPressed(Key::Space))
			m_MoveDirection += glm::vec3(0.0f, 1.0f, 0.0f);
		if (Input::IsKeyPressed(Key::LeftControl))
			m_MoveDirection += glm::vec3(0.0f, -1.0f, 0.0f);

		if (glm::length(m_MoveDirection) > 0.01f)
		{
			m_MoveDirection = glm::normalize(m_MoveDirection);
			float speed = m_Settings.TranslateSpeed;
			if (m_IsSprinting)
				speed *= m_Settings.SprintSpeedBoost;
			m_Velocity = m_MoveDirection * speed;
		}
		else
		{
			m_Velocity = glm::vec3(0.0f);
		}

		m_Camera.Translate(m_Camera.GetCameraRight() * m_Velocity.x * (float)ts);
		m_Camera.Translate(m_Camera.GetCameraUp() * m_Velocity.y * (float)ts);
		m_Camera.Translate(m_Camera.GetCameraForward() * m_Velocity.z * (float)ts);
	}

	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		glm::vec2 currentMousePos = { e.GetX(), e.GetY() };

		if (m_FirstMouseMove)
		{
			m_LastMousePos = currentMousePos;
			m_FirstMouseMove = false;
			return false;
		}

		glm::vec2 mouseDelta = currentMousePos - m_LastMousePos;
		m_LastMousePos = currentMousePos;

		float pitchDelta = mouseDelta.y * m_Settings.MouseSensitivity;
		float yawDelta = mouseDelta.x * m_Settings.MouseSensitivity;

		if (m_Settings.InvertPitch)
			pitchDelta *= -1.0f;

		m_Camera.Rotate(pitchDelta, yawDelta);

		return false;
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Camera.SetProjection(m_Camera.GetFOV() - e.GetYOffset(), m_Camera.GetAspectRatio(), m_Camera.GetNearClip(), m_Camera.GetFarClip());
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_Camera.SetProjection(m_Camera.GetFOV(), (float)e.GetWidth() / (float)e.GetHeight(), m_Camera.GetNearClip(), m_Camera.GetFarClip());
		return false;
	}
}
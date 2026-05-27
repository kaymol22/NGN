#pragma once
#include "PerspectiveCamera.h"
#include "Core/Timestep.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

namespace NGN
{
	struct ControllerSettings
	{
		float MouseSensitivity = 0.1f;
		float TranslateSpeed = 5.0f;
		float SprintSpeedBoost = 2.0f;
		float MinPitch = -89.0f;
		float MaxPitch = 89.0f;
		bool InvertPitch = false;
	};

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(const ControllerSettings& settings = {});
		~PerspectiveCameraController() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetSettings(const ControllerSettings& settings) { m_Settings = settings; }
		const ControllerSettings& GetSettings() { return m_Settings; }

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		glm::vec3 GetVelocity() const { return m_Velocity; }
		bool IsMoving() const { return glm::length(m_Velocity) > 0.0f; }
	
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		void UpdateMovement(Timestep ts);
		/*void UpdateRotation();*/

	private:
		ControllerSettings m_Settings;
		PerspectiveCamera m_Camera;

		// State
		glm::vec3 m_Velocity = { 0.0f, 0.0f, 0.0f };
		bool m_FirstMouseMove = true;
		glm::vec2 m_LastMousePos = { 0.0f, 0.0f };
		bool m_IsSprinting = false;
		glm::vec3 m_MoveDirection = { 0.0f, 0.0f, 0.0f };
	};
}
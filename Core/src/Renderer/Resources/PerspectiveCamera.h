#pragma once

#include "Camera.h"
#include <glm/glm.hpp>

namespace NGN
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov = 45.0f, float aspectRatio = 16.0f / 9.0f, float nearClip = 0.1f, float farClip = 100.0f);
		~PerspectiveCamera() = default;

		void SetProjection(float fov, float aspectRatio, float nearClip, float farClip);

		glm::vec3 GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
		void SetPitch(float pitch) { m_Pitch = pitch; RecalculateViewMatrix(); }
		void SetYaw(float yaw) { m_Yaw = yaw; RecalculateViewMatrix(); }

		void SetMinPitch(float minPitch) { m_MinPitch = minPitch; }
		void SetMaxPitch(float maxPitch) { m_MaxPitch = maxPitch; }

		glm::vec3& GetCameraForward() { return m_Forward; }
		glm::vec3& GetCameraRight() { return m_Right; }
		glm::vec3& GetCameraUp() { return m_Up; }

		float GetFOV() const { return m_Fov; }
		float GetAspectRatio() const { return m_AspectRatio; }
		float GetNearClip() const { return m_NearClip; }
		float GetFarClip() const { return m_FarClip; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		const glm::mat4& GetInverseViewMatrix() const { return glm::inverse(m_ViewMatrix); }

		void Translate(const glm::vec3& transation) { m_Position += transation; RecalculateViewMatrix(); }
		void Rotate(float pitchDelta, float yawDelta);

	private:
		void RecalculateViewMatrix();
		void UpdateDirectionVectors();

	private:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;

		float m_MaxPitch = 89.0f; // Prevent gimbal lock
		float m_MinPitch = -89.0f;

		glm::vec3 m_Forward = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };

		float m_Fov = 45.0f;
		float m_AspectRatio = 16.0f / 9.0f;
		float m_NearClip = 0.1f;
		float m_FarClip = 1000.0f;
	};
}
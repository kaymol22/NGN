#include "ngnpch.h"
#include "PerspectiveCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace NGN
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		SetProjection(fov, aspectRatio, nearClip, farClip);
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_Fov = fov;
		m_AspectRatio = aspectRatio;
		m_NearClip = nearClip;
		m_FarClip = farClip;

		m_ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			aspectRatio,
			nearClip,
			farClip
		);
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		UpdateDirectionVectors();

		glm::vec3 target = m_Position + m_Forward;
		m_ViewMatrix = glm::lookAt(m_Position, target, m_Up);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::UpdateDirectionVectors()
	{
		// Convvert to dir vectors
		float pitchRad = glm::radians(m_Pitch);
		float yawRad = glm::radians(m_Yaw);

		m_Forward.x = sin(yawRad) * cos(pitchRad);
		m_Forward.y = -sin(pitchRad); // Positive pitch should look up
		m_Forward.z = -cos(yawRad) * cos(pitchRad);
		m_Forward = glm::normalize(m_Forward);

		// Update new right and up vectors
		m_Right = glm::normalize(glm::cross(m_Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
	}

	void PerspectiveCamera::Rotate(float pitchDelta, float YawDelta)
	{
		m_Pitch += pitchDelta;
		m_Pitch = glm::clamp(m_Pitch, m_MinPitch, m_MaxPitch);
		m_Yaw += YawDelta;
		RecalculateViewMatrix();
	}
}
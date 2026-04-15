#include "ngnpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace NGN
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspFOV = fov;
		m_AspectRatio = aspectRatio;
		m_PerspNear = nearClip;
		m_PerspFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthoSize = size;
		m_OrthoNear = nearClip;
		m_OrthoFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		NGN_CORE_ASSERT(width > 0 && height > 0, "Viewport dimensions must be greater than zero");
		m_ViewportWidth = static_cast<float>(width);
		m_ViewportHeight = static_cast<float>(height);
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateViewMatrix(const glm::vec3& position, const glm::vec3& rotation)
	{
		// Convert euler angles to radians 
		float pitch = glm::radians(rotation.x);
		float yaw = glm::radians(rotation.y);

		glm::vec3 forward;
		forward.x = sin(yaw) * cos(pitch);
		forward.y = -sin(pitch);
		forward.z = -cos(yaw) * cos(pitch);

		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::vec3 up = glm::normalize(glm::cross(right, forward));

		m_ViewMatrix = glm::lookAt(position, position + forward, up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	
	void SceneCamera::RecalculateProjection()
	{ 
		if (m_ProjectionType == NGN::ProjectionType::Perspective)
			m_ProjectionMatrix = glm::perspective(m_PerspFOV, m_AspectRatio, m_PerspNear, m_PerspFar);

		else
		{
			float orthoLeft = -m_OrthoSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthoSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthoSize * 0.5f;
			float orthoTop = m_OrthoSize * 0.5f;
			m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthoNear, m_OrthoFar);
		}
	}
}
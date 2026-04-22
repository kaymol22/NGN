#include "ngnpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace NGN
{
	SceneCamera::SceneCamera()
	{
		SetProjectionType(m_ProjectionType);
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

	void SceneCamera::RecalculateViewMatrix(const glm::vec3& position, const glm::quat& rotation)
	{
		// Pull from transform component
		m_Position = position;
		m_Rotation = rotation;

		UpdateDirectionVectors();

		// Use lookAt for both orthographic and perspective
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void SceneCamera::UpdateDirectionVectors()
	{
		// Calc new forward by applying rotation to world forward 
		constexpr glm::vec3 worldForward = { 0.0f, 0.0f, -1.0f };
		m_Forward = glm::normalize(glm::rotate(m_Rotation, worldForward));

		// Derive right and up vectors using cross product
		// Right-handed coordinate system with Y-up**
		constexpr glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f };
		m_Right = glm::normalize(glm::cross(worldUp, m_Forward));

		// Up = Forward × Right (recalculate based on rotated forward and computed right)
		m_Up = glm::normalize(glm::cross(m_Forward, m_Right));
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
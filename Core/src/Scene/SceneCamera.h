#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Renderer/Camera.h"

namespace NGN
{
	enum class ProjectionType
	{
		Perspective,
		Orthographic
	};

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		~SceneCamera() = default;

		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }
		ProjectionType GetProjectionType() const { return m_ProjectionType; }

		void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);

		// Viewport
		void SetViewportSize(uint32_t width, uint32_t height);
		float GetViewportWidth() const { return m_ViewportWidth; }
		float GetViewportHeight() const { return m_ViewportHeight; }

		// Perspective 
		float GetPerspectiveFOV() const { return m_PerspFOV; }
		void SetPerspectiveFOV(float fov) { m_PerspFOV = fov; RecalculateProjection(); }
		float GetPerspectiveNear() const { return m_PerspNear; }
		void SetPerspectiveNear(float nearClip) { m_PerspNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFar() const { return m_PerspFar; }
		void SetPerspectiveFar(float farClip) { m_PerspFar = farClip; RecalculateProjection(); }

		// Orthographic
		float GetOrthographicSize() const { return m_OrthoSize; }
		void SetOrthographicSize(float size) { m_OrthoSize = size; RecalculateProjection(); }
		float GetOrthographicNear() const { return m_OrthoNear; }
		void SetOrthographicNear(float nearClip) { m_OrthoNear = nearClip; }
		float GetOrthographicFar() const { return m_OrthoFar; }
		void SetOrthographicFar(float farClip) { m_OrthoFar = farClip; RecalculateProjection(); }

		// Cached transform & dir vectors foor faster retrieval by controller system
		glm::vec3 GetPosition() const { return m_Position; }
		glm::quat GetRotation() const { return m_Rotation; }
		glm::vec3 GetForward() const { return m_Forward; }
		glm::vec3 GetRight() const { return m_Right; }
		glm::vec3 GetUp() const { return m_Up; }

		void RecalculateViewMatrix(const glm::vec3& position, const glm::quat& rotation);
		void UpdateDirectionVectors();

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		// Cache values from transform component
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::quat m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quat

		glm::vec3 m_Forward = { 0.0f, 0.0f, -1.0f }; // neg z axis for opengl
		glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };

		float m_ViewportWidth = 0.0f;
		float m_ViewportHeight = 0.0f;
		float m_AspectRatio = 0.0f;

		float m_PerspFOV = glm::radians(45.0f);
		float m_PerspNear = 0.01f;
		float m_PerspFar = 1000.0f;

		float m_OrthoSize = 10.0f;
		float m_OrthoNear = -1.0f;
		float m_OrthoFar = 1.0f;
	};
}
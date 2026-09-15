#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace NGN
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_ProjectionMatrix(projection) {}
		virtual ~Camera() = default;

		virtual const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;

		glm::vec3 GetPosition() const { return m_Position; }
		glm::quat GetRotation() const { return m_Rotation; }
		glm::vec3 GetForward() const { return m_Forward; }
		glm::vec3 GetRight() const { return m_Right; }
		glm::vec3 GetUp() const { return m_Up; }

	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::quat m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		glm::vec3 m_Forward = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
	};
}
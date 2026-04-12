#pragma once

#include <glm/glm.hpp>

namespace NGN
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_Projection(projection) {}
		virtual ~Camera() = default;

		virtual const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;

	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}
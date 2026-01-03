#pragma once

#include <glm/glm.hpp>

namespace NGN::Math
{
	bool DecomposeTransform(
		const glm::mat4& transform,
		glm::vec3& translation,
		glm::vec3& rotation,
		glm::vec3& scale
	);

	float Lerp(float a, float b, float t);
	glm::vec2 Lerp(const glm::vec2& a, const glm::vec2& b, float t);

	float Clamp(float value, float min, float max);
}
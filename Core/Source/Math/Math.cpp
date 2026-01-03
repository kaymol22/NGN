#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace NGN::Math
{

	bool DecomposeTransform(
		const glm::mat4& transform,
		glm::vec3& translation,
		glm::vec3& rotation,
		glm::vec3& scale
	)
	{
		using namespace glm;

		mat4 localMatrix(transform);

		// Normalize matrix
		if (epsilonEqual(localMatrix[3][3], 0.0f, epsilon<float>()))
			return false;

		// Remove perspective if there first
		if (
			epsilonNotEqual(localMatrix[0][3], 0.0f, epsilon<float>()) ||
			epsilonNotEqual(localMatrix[1][3], 0.0f, epsilon<float>()) ||
			epsilonNotEqual(localMatrix[2][3], 0.0f, epsilon<float>()))
		{
			localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = 0.0f;
			localMatrix[3][3] = 1.0f;
		}

		// Extract translation
		translation = vec3(localMatrix[3]);
		localMatrix[3] = vec4(0, 0, 0, localMatrix[3].w);

		// Extract scale
		vec3 row[3] = {};
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				row[i][j] = localMatrix[i][j];

		scale.x = length(row[0]);
		row[0] = normalize(row[0]);

		scale.y = length(row[1]);
		row[1] = normalize(row[1]);

		scale.z = length(row[2]);
		row[2] = normalize(row[2]);

		// Extract rotation ( Euler, radians )
		rotation.y = asin(-row[0][2]);

		if (cos(rotation.y) != 0.0f)
		{
			rotation.x = atan2(row[1][2], row[2][2]);
			rotation.z = atan2(row[0][1], row[0][0]);
		}
		else
		{
			rotation.x = atan2(-row[2][0], row[1][1]);
			rotation.z = 0.0f;
		}

		return true;
	}

	float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	glm::vec2 Lerp(const glm::vec2& a, const glm::vec2& b, float t)
	{
		return a + (b - a) * t;
	}

	float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}
}
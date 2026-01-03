#pragma once

#include <glm/glm.hpp>
#include "Component.h"

namespace NGN
{
	struct TransformComponent : public Component {
		glm::vec3 Translation	{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation		{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale			{ 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransform() const;
	};
}
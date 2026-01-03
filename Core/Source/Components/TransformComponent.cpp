#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

namespace NGN
{
	glm::mat4 TransformComponent::GetTransform() const
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), Translation);
		transform = glm::rotate(transform, Rotation.x, glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, Rotation.y, glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, Rotation.z, glm::vec3(0, 0, 1));
		transform = glm::scale(transform, Scale);
		return transform;
	}
}
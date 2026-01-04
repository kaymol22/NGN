#pragma once

#include <glm/glm.hpp>

namespace NGN
{
	struct TransformComponent {
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };
	};

	struct SpriteComponent
	{
		uint32_t TextureID = 0;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct CameraComponent
	{
		bool Primary = true;
	};

	struct TagComponent
	{
		// debugging and editor 
		std::string Tag;
	};
}
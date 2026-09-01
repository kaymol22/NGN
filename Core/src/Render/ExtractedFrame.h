#pragma once

#include "ResourceManagement/CPU/ResourceHandle.h"
#include "ResourceManagement/CPU/Types/Texture.h"

namespace NGN
{
	struct ExtractedSprite
	{
		glm::mat4 transform;
		RS::ResourceHandle<RS::Texture> Texture;
		glm::vec4 Color;
		float TilingFactor;
		glm::vec3 BoundsMin;
		glm::vec3 BoundMax;
	};

	struct ExtractedFrame
	{
		std::vector<ExtractedSprite> Sprites;

		void Clear() { Sprites.clear(); }
	};
}
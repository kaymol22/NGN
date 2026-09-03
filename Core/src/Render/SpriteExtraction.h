#pragma once

#include "ResourceManagement/CPU/ResourceHandle.h"
#include "ResourceManagement/CPU/Types/Texture.h"

namespace NGN
{
	class Scene;

	struct SpriteRenderItem
	{
		glm::mat4 transform{ 1.0f };
		RS::ResourceHandle<RS::Texture> Texture;
		glm::vec4 Color;
		float TilingFactor;
	};

	std::vector<SpriteRenderItem> ExtractVisibleSprites(Scene& scene);
}
#pragma once
#include <glm/glm.hpp>

struct RenderItem
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 inverseModelMatrix = glm::mat4(1.0f);
	glm::vec4 aabbMin = glm::vec4(0.0f);
	glm::vec4 aabbMax = glm::vec4(0.0f);

	int32_t meshIndex = 0;
	int32_t baseColorTextureIndex = 0;
	int32_t normalMapTextureIndex = 0;
	int32_t rmaTextureIndex = 0;

	int32_t objectType = 0;
	
	float tintColorR = 1.0f;
	float tintColorG = 1.0f;
	float tintColorB = 1.0f;
	bool castShadows = false;
};
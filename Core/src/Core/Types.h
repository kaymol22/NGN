#pragma once
#include <glm/glm.hpp>

#include "Renderer/Resources/Mesh.h"

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
	
	uint32_t castShadows = 1;
	uint32_t skinned = 0;

	NGN::Mesh* mesh = nullptr;
};

struct RenderItem2D
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	float colorTintR = 1.0f;
	float colorTintG = 1.0f;
	float colorTintB = 1.0f;
	int textureIndex = -1;
	int baseVertex = 0;
	int baseIndex = 0;
};

struct SpriteSheetRenderItem
{
	glm::vec4 position;
	glm::vec4 rotation; // Quaternion as vec4
	glm::vec4 scale;
	glm::vec4 aabbMin;
	glm::vec4 aabbMax;

	float uOffset;
	float vOffset;
	int textureIndex;
	int frameIndex;

	float paddingX;
	float paddingY;
};

// Flat - 32 bytes
#pragma pack(push, 1)
struct BVHNode
{
	glm::vec3 aabbMin;
	uint32_t firstChildOrPrimitive; // leaf: index of first primitive, internal: index of first child node
	glm::vec3 aabbMax;
	uint32_t primitiveCount; // 0 for internal nodes, > 0 for leaf nodes
};
#pragma pack(pop)

// objectID maps back to scene entity to construct RenderItem
struct PrimitiveInstance
{
	uint64_t objectID;
	glm::vec3 worldAabbBoundsMin;
	glm::vec3 worldAabbBoundsMax;
	glm::vec3 worldAabbCentre;
	glm::mat4 worldTransform;
	glm::mat4 inverseWorldTransform;
};

// BVH for entire scene owned by SceneBVH - to upload to GPU as SSBO
struct SceneBvh
{
	std::vector<BVHNode> Nodes;
	std::vector<PrimitiveInstance> Primitives;
};
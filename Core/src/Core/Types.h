#pragma once
#include <glm/glm.hpp>

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
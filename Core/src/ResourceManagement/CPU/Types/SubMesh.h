#pragma once
#include "Render/RendererEnums.h"

struct SubMesh
{
	uint32_t meshId = 0; // Key into meshbuffer table
	uint32_t materialSlot = 0;
	BlendingMode blendMode = BlendingMode::DEFAULT;
	glm::mat4 localTransform = glm::mat4(1.0f);
	int32_t parentIndex = -1; // -1 if no parent, otherwise index into submesh array
};

struct RawSubMeshData
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::string name;
	glm::mat4 localTransform = glm::mat4(1.0f);
	int32_t parentIndex = -1;
	BlendingMode blendMode = BlendingMode::DEFAULT;
};
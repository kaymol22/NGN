#pragma once
#include "ResourceManagement/CPU/Types/GenericMesh.h"
#include "ResourceManagement/CPU/ResourceHandle.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>

struct ViewportData
{
	glm::mat4 viewMatrix;
	glm::mat4 inverseViewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 inverseProjectionMatrix;
	glm::mat4 viewProjectionMatrix;
	glm::mat4 viewProjectionReverseZ;
	glm::mat4 inverseViewProjectionMatrix;
	glm::mat4 inverseViewProjectionReverseZ;
	glm::mat4 prevViewProjectionMatrix = glm::mat4(1.0f);

	glm::vec4 frustumPlane0;
	glm::vec4 frustumPlane1;
	glm::vec4 frustumPlane2;
	glm::vec4 frustumPlane3;
	glm::vec4 frustumPlane4;
	glm::vec4 frustumPlane5;

	glm::vec4 viewPosition;
	glm::vec4 camForward;
	glm::vec4 camUp;
	glm::vec4 camRight;

	bool isPerspective = true;
	float fov = 45.0f;
	float orthographicSize = 10.0f;
};

struct RendererData
{
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float gBufferWidth = 0.0f;
	float gBufferHeight = 0.0f;

	float normalizedMouseX = 0.0f;
	float normalizedMouseY = 0.0f;
};

struct DrawItem
{
	uint32_t objectIndex;
	uint32_t meshIndex;
	uint32_t indexCount;
	uint32_t firstIndex;
	uint32_t baseVertex;
};

struct RenderObject
{
	uint64_t objectId;
	glm::mat4 transform;
	glm::vec4 color;
	glm::vec3 boundsMin;
	glm::vec3 boundsMax;
	uint32_t firstRenderItem;
	uint32_t renderItemCount;
};

struct RenderItem {
	uint64_t objectIndex;

	uint64_t meshId;
	uint64_t materialId;
};

struct BlitRegion {
	int32_t originX = 0;
	int32_t originY = 0;
	int32_t width = 0;
	int32_t height = 0;
};

struct BlitRect {
	int32_t x0 = 0;
	int32_t y0 = 0;
	int32_t x1 = 0;
	int32_t y1 = 0;
};
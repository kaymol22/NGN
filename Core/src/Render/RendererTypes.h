#pragma once
#include "RendererEnums.h"
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
	glm::mat4 inverseViewProjectionMatrix;

	glm::vec4 viewPosition;
	glm::vec4 camForward;
	glm::vec4 camUp;
	glm::vec4 camRight;

	float fov = 45.0f;
	float nearPlane = 0.01f;
	float farPlane = 1000.0f;
	float orthographicSize = 10.0f;
};

struct GPUObjectData {
	glm::mat4 transform;
	glm::vec4 color;
};

// CPU side representation of a renderable object in the scene
struct RenderItem
{
	uint64_t objectId; // ID component
	uint32_t meshId; // Mesh buffer ID
	BlendingMode blendMode;
	glm::vec3 boundsMin;
	glm::vec3 boundsMax;
	GPUObjectData gpuData;
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
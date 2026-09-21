#version 460 core
#include "common/GL_binding_indices.glsl"
#include "common/types.glsl"

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

layout(std430, binding = SSBO_IDX_VIEWPORT_DATA) readonly restrict buffer viewportDataBuffer 
{
	ViewportData viewportData;
};

layout(std430, binding = SSBO_IDX_SCENE_RENDER_ITEMS) readonly buffer sceneRenderItemsBuffer
{
	RenderItem sceneRenderItems[];
};

layout(std430, binding = SSBO_IDX_DRAW_RENDER_ITEM_INDICES) readonly buffer drawRenderItemIndicesBuffer
{
	uint drawRenderItemIndices[];
};

out vec3 Normal;
out vec4 WorldPos;
out flat vec4 Color;

void main()
{
	uint slot = uint(gl_BaseInstance) + uint(gl_InstanceID);
	uint renderItemIndex = drawRenderItemIndices[slot];
	RenderItem item = sceneRenderItems[renderItemIndex];

	mat4 modelMatrix = item.transform;
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	Normal = normalize(normalMatrix * vNormal);
	Color = item.color;
	WorldPos = modelMatrix * vec4(vPosition, 1.0);

	gl_Position = viewportData.viewProjectionMatrix * WorldPos;
}
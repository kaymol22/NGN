#pragma once

enum class RendererAPIType
{
	OPENGL,
	VULKAN,
	UNDEFINED
};

enum class ObjectType : uint16_t
{
	NO_TYPE = 0,
	STATIC_MESH,
	SKELETAL_MESH,
	SKINNED_MESH,
	
	UNDEFINED
};
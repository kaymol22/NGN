#pragma once

enum class API
{
	OPENGL,
	VULKAN,
	UNDEFINED
};

enum class WindowedMode
{
	WINDOWED,
	FULLSCREEN
};

enum class ObjectType : uint16_t
{
	NO_TYPE = 0,
	STATIC_MESH = 1,
	SKELETAL_MESH = 2,
	SKINNED_MESH = 3,
	SPRITE = 4,
	TRANSPERANT_OBJECT = 5, // Have to handle this separately
	
	UNDEFINED = 6
};

enum class PassType : uint8_t
{
	ShadowMap = 0,
	Geometry = 1,
	Lighting = 2,
	Composition = 3,
	UI = 4,

	Count = 5
};

enum class UploadState : uint8_t {
	NOT_REQUESTED,
	QUEUED,
	UPLOADING,
	UPLOADED,
	FAILED
};
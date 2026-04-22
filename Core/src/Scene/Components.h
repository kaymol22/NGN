#pragma once

#include "SceneCamera.h"

#include "Core/UUID.h"
#include "Renderer/Texture.h"
#include "Renderer/SubTexture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace NGN
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(const UUID& id) : ID(id) {}
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent {
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::quat Rotation{ 0.0f, 0.0f, 0.0f, 1.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		uint32_t TextureID = 0;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		Ref<SubTexture2D> SubTexture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;

		SpriteRendererComponent(const Ref<Texture2D>& texture) : Texture(texture) {}
		SpriteRendererComponent(const Ref<SubTexture2D>& texture) : SubTexture(texture) {}
		SpriteRendererComponent(const glm::vec4 color) : Color(color) {}
		SpriteRendererComponent(const Ref<Texture2D>& texture, const glm::vec4 color) : Texture(texture), Color(color) {}
		SpriteRendererComponent(const Ref<SubTexture2D>& texture, const glm::vec4 color) : SubTexture(texture), Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;

		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() : Camera() {}  // Explicitly initialize
		CameraComponent(const CameraComponent&) = default;
	};

	struct PlayerControllerComponent
	{
		float MouseSensitivity = 0.1f;
		float TranslateSpeed = 5.0f;
		float SprintMultiplier = 2.0f;
		float MinPitch = -89.0f;
		float MaxPitch = 89.0f;
		bool InvertPitch = false;

		// Runtime state
		float pitchDeg = 0.0f;
		float yawDeg = 0.0f;
		bool IsActive = true;
		bool IsGrounded = false; // Will keep jump disabled for now - need phys implementation first

		PlayerControllerComponent() = default;
		PlayerControllerComponent(const PlayerControllerComponent&) = default;
	};
}
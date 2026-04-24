#pragma once

#include "SceneCamera.h"

#include "Core/UUID.h"
#include "Renderer/Texture.h"
#include "Renderer/SubTexture.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"

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

		// Bounding box for culling (in world space)
		glm::vec3 boundsMin{ -0.5f, -0.5f, 0.0f };
		glm::vec3 boundsMax{ 0.5f, 0.5f, 0.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;

		SpriteRendererComponent(const Ref<Texture2D>& texture) : Texture(texture) {}
		SpriteRendererComponent(const Ref<SubTexture2D>& texture) : SubTexture(texture) {}
		SpriteRendererComponent(const glm::vec4 color) : Color(color) {}
		SpriteRendererComponent(const Ref<Texture2D>& texture, const glm::vec4 color) : Texture(texture), Color(color) {}
		SpriteRendererComponent(const Ref<SubTexture2D>& texture, const glm::vec4 color) : SubTexture(texture), Color(color) {}

		// Update bounding box based on transform and sprite size
		void UpdateBounds(const TransformComponent& transform, const glm::vec2& spriteSize = glm::vec2(1.0f, 1.0f))
		{
			glm::vec3 halfSize = glm::vec3(
				spriteSize.x * transform.Scale.x * 0.5f,
				spriteSize.y * transform.Scale.y * 0.5f,
				0.01f  // Small z for 2D sprites
			);
			boundsMin = transform.Translation - halfSize;
			boundsMax = transform.Translation + halfSize;
		}
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

	struct MeshComponent
	{
		Ref<NGN::Mesh> MeshRef;
		Ref<Shader> ShaderRef;
		glm::vec4 Color = glm::vec4(1.0f);

		// Bounding box for culling (in world space)
		glm::vec3 boundsMin = glm::vec3(-0.5f);
		glm::vec3 boundsMax = glm::vec3(0.5f);

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;

		MeshComponent(const Ref<NGN::Mesh>& mesh, const Ref<Shader>& shader = nullptr, const glm::vec4& color = glm::vec4(1.0f))
			: MeshRef(mesh), ShaderRef(shader), Color(color)
		{
			if (mesh)
			{
				boundsMin = mesh->GetBoundsMin();
				boundsMax = mesh->GetBoundsMax();
			}
		}

		// Update bounding box based on transform
		void UpdateBounds(const TransformComponent& transform)
		{
			if (!MeshRef)
				return;

			glm::vec3 meshMin = MeshRef->GetBoundsMin();
			glm::vec3 meshMax = MeshRef->GetBoundsMax();

			glm::vec3 scaledMin = meshMin * transform.Scale;
			glm::vec3 scaledMax = meshMax * transform.Scale;

			boundsMin = transform.Translation + glm::min(scaledMin, scaledMax);
			boundsMax = transform.Translation + glm::max(scaledMin, scaledMax);
		}
	};
}
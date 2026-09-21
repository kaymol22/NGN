#pragma once

#include "SceneCamera.h"
#include "Render/Frustum.h"

#include "Core/UUID.h"

#include "ResourceManagement/CPU/ResourceManager.h"
#include "ResourceManagement/CPU/ResourceHandle.h"
#include "ResourceManagement/CPU/Types/Texture.h"
#include "ResourceManagement/CPU/Types/Mesh.h"
#include "ResourceManagement/CPU/Types/SubMesh.h"

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
		glm::quat Rotation = glm::identity<glm::quat>();
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };
		glm::mat4 TransformMatrix{ 1.0f };

		bool IsDirty = true;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation), IsDirty(true) {}
		void SetTranslation(const glm::vec3& translation) { Translation = translation; IsDirty = true; }
		void SetRotation(const glm::quat& rotation) { Rotation = rotation; IsDirty = true; }
		void SetScale(const glm::vec3& scale) { Scale = scale; IsDirty = true; }
		void Translate(const glm::vec3& delta) { Translation += delta; IsDirty = true; }
		void Rotate(const glm::quat& delta) { Rotation = delta * Rotation; IsDirty = true; }
		void RotateEuler(const glm::vec3& eulerAngles) { Rotation = glm::quat(eulerAngles) * Rotation; IsDirty = true; }

		const glm::mat4 GetTransformMatrix() { return TransformMatrix; }

		void UpdateMatrix()
		{
			if (!IsDirty) return;

			TransformMatrix = 
				glm::translate(glm::mat4(1.0f), Translation) *
				glm::toMat4(Rotation) *
				glm::scale(glm::mat4(1.0f), Scale);

			IsDirty = false;
		}
	};

	struct SpriteComponent
	{
		friend class Application;

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		RS::ResourceHandle<RS::Texture> Texture;
		float TilingFactor = 1.0f;

		glm::vec3 boundsMin{ -0.5f, -0.5f, 0.0f };
		glm::vec3 boundsMax{ 0.5f, 0.5f, 0.0f };

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;

		explicit SpriteComponent(const glm::vec4& color);
		explicit SpriteComponent(const std::string& id);

		void UpdateBounds(const TransformComponent& transform, 
			const glm::vec2& spriteSize = glm::vec2(1.0f, 1.0f))
		{
			glm::vec3 halfSize = glm::vec3(
				spriteSize.x * transform.Scale.x * 0.5f,
				spriteSize.y * transform.Scale.y * 0.5f,
				0.01f
			);
			boundsMin = transform.Translation - halfSize;
			boundsMax = transform.Translation + halfSize;
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		Frustum ViewFrustum;

		bool Primary = true;
		bool FixedAspectRatio = false;
		bool ViewDirty = true;

		CameraComponent();
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
		RS::ResourceHandle<RS::MeshAsset> Mesh;
		glm::vec4 Color = glm::vec4(1.0f);
		std::vector<SubMesh> SubMeshes;
		glm::vec3 boundsMin = glm::vec3(-0.5f);
		glm::vec3 boundsMax = glm::vec3(0.5f);

		MeshComponent() = default;
		explicit MeshComponent(const std::string& id, const glm::vec4& color = glm::vec4(1.0f));
		MeshComponent(const MeshComponent&) = default;

		//// Update bounding box based on transform
		void UpdateBounds(const TransformComponent& transform)
		{
			if (!Mesh)
				return;

			glm::vec3 meshMin = Mesh->GetBoundsMin();
			glm::vec3 meshMax = Mesh->GetBoundsMax();

			glm::vec3 scaledMin = meshMin * transform.Scale;
			glm::vec3 scaledMax = meshMax * transform.Scale;

			boundsMin = transform.Translation + glm::min(scaledMin, scaledMax);
			boundsMax = transform.Translation + glm::max(scaledMin, scaledMax);
		}
	};

	struct RigidBodyComponent
	{
		float Mass = 1.0f;
		glm::vec3 Velocity = glm::vec3(0.0f);

	};
}
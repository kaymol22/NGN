#include "ngnpch.h"
#include "PlayerControllerSystem.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Core/KeyCodes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


namespace NGN
{
	PlayerControllerSystem::PlayerControllerSystem()
	{
		// Set up default input bindings - can be overridden by player
		// TODO: Load from config file or allow rebinding at runtime
		m_InputBindings.SetDefaults();
	}
	void PlayerControllerSystem::OnUpdate(Scene& scene, Timestep ts)
	{
		NGN_PROFILE_FUNCTION();

		// State update once per frame
		m_InputMapper.Update(m_CurrentInputState, m_InputBindings);

		auto& registry = scene.GetRegistry();
		auto view = registry.view<TransformComponent, PlayerControllerComponent>();
		
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& controller = view.get<PlayerControllerComponent>(entity);

			if (!controller.IsActive)
				continue;

			CameraComponent* camComp = nullptr;
			if (registry.any<CameraComponent>(entity))
				camComp = &registry.get<CameraComponent>(entity);

			UpdatePlayer(transform, controller, camComp, m_CurrentInputState, ts);
		}
	}

	void PlayerControllerSystem::UpdatePlayer(
		TransformComponent& transform,
		PlayerControllerComponent& controller,
		CameraComponent* camera,
		const InputState& input, 
		Timestep ts)
	{
		NGN_PROFILE_FUNCTION();
		
		// Reset
		glm::vec3 moveDir = { 0.0f, 0.0f, 0.0f };

		// Get forward and right vectors from current rotation
		if (glm::length(input.MovementInput) > 0.0f)
		{
			// Convert transform comp Euler rotation to quat
			glm::quat q = glm::normalize(transform.Rotation);

			glm::vec3 forward = glm::rotate(q, glm::vec3(0.0f, 0.0f, -1.0f));
			glm::vec3 right = glm::rotate(q, glm::vec3(1.0f, 0.0f, 0.0f));

			moveDir = forward * input.MovementInput.y + right * input.MovementInput.x;
		}

		// Jump
		if (input.IsJumping && controller.IsGrounded)
		{
			moveDir.y += 1.0f; // Placeholder - will need physics implementation for actual jump force
			controller.IsGrounded = false;
		}

		// Sprint
		float speed = controller.TranslateSpeed;
		// TODO: Add acceleration/deceleration for smoother start/stop and sprint transitions
		// Factor in direction
		if (input.IsSprinting)
			speed *= controller.SprintMultiplier;

		// Apply
		transform.Translation += moveDir * speed * ts.GetSeconds();

		// Rotation
		if (glm::length(input.LookDelta) > 0.0f)
		{
			glm::vec2 lookDelta = input.LookDelta * controller.MouseSensitivity;
			if (controller.InvertPitch)
				lookDelta.y = -lookDelta.y;

			controller.yawDeg += lookDelta.x;
			controller.pitchDeg += lookDelta.y;
			controller.pitchDeg = glm::clamp(controller.pitchDeg, controller.MinPitch, controller.MaxPitch);

			transform.Rotation = EulerToQuaternion(controller.pitchDeg, controller.yawDeg);
		}
		 if (camera)
			 camera->Camera.RecalculateViewMatrix(transform.Translation, transform.Rotation);
	}

	glm::quat PlayerControllerSystem::EulerToQuaternion(float pitchDeg, float yawDeg)
	{
		float pitchRad = glm::radians(pitchDeg);
		float yawRad = glm::radians(yawDeg);
		glm::quat qPitch = glm::angleAxis(pitchRad, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat qYaw = glm::angleAxis(yawRad, glm::vec3(0.0f, 1.0f, 0.0f));
		return qYaw * qPitch; // Yaw applied first - not commutative
	}
}
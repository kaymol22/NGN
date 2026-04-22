#pragma once
#include "System.h"
#include "Scene/Components.h"
#include "Core/Timestep.h"
#include "Input/InputBindings.h"
#include "Input/InputMapper.h"
#include "Input/InputState.h"

namespace NGN
{
	class PlayerControllerSystem : public System
	{
	public:
		PlayerControllerSystem();
		void OnUpdate(Scene& scene, Timestep ts) override;

	private:
		void UpdatePlayer(
			TransformComponent& transform,
			PlayerControllerComponent& controller,
			CameraComponent* camera,
			const InputState& input,
			Timestep ts
		);
		
		glm::quat EulerToQuaternion(float pitchDeg, float yawDeg);

		// Input management
		InputBindings m_InputBindings;
		KeyboardMouseInputMapper m_InputMapper;
		InputState m_CurrentInputState;
	};
}
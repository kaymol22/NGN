#include "CameraSystem.h"

namespace NGN {
	void CameraSystem::OnCreate(Scene& scene) {

	}

	void CameraSystem::OnUpdate(Scene& scene, Timestep ts) {

		auto view = scene.GetRegistry().view<CameraComponent, TransformComponent>();

		for (auto entity : view) {
			auto& cameraComp = view.get<CameraComponent>(entity);
			auto& transform = view.get<TransformComponent>(entity);

			if (cameraComp.ViewDirty) {
				cameraComp.Camera.RecalculateViewMatrix(transform.Translation, transform.Rotation);
				const glm::mat4& vm = cameraComp.Camera.GetViewMatrix();
				/*NGN_CORE_INFO("View[0]: {} {} {} {} | rotation quat: {} {} {} {}",
					vm[0][0], vm[0][1], vm[0][2], vm[0][3],
					transform.Rotation.w, transform.Rotation.x, transform.Rotation.y, transform.Rotation.z);*/

				glm::mat4 viewProj = cameraComp.Camera.GetViewProjectionMatrix();
				cameraComp.ViewFrustum.Update(viewProj);

				cameraComp.ViewDirty = false;
			}
		}
	}
}

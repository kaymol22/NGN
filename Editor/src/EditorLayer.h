#pragma once

#include <glm/glm.hpp>

#include "Core/Layer.h"
#include "Scene/eScene.h"

namespace NGN
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		void OnUpdate(Timestep ts);
		void OnImGuiRender();
		void OnEvent(Event e);

	private:
		/*NGN::OrthographicCameraController m_CameraController;*/
		
		bool ViewportFocused = false, ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		Ref<Scene> m_ActiveScene = nullptr;
	};
}
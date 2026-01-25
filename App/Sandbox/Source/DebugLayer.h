#pragma once

#include "Core/Layer.h"
#include "Core/Log.h"
//#include "Core/Application.h"

#include <imgui.h>

class DebugLayer : public NGN::Layer
{
public:
	DebugLayer()
		: Layer(
			"Debug",
			NGN::LayerFlags::UsesImGui | NGN::LayerFlags::Debug
		)
	{}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text("FPS: %.1f", 1.0f / NGN::Application::Get().GetTimestep());
		ImGui::End();
	}
};
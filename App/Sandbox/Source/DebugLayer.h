#pragma once

#include "Core/Layer.h"
#include "Core/Log.h"
//#include "Core/Application.h"
#include "Debug/ChromeProfiling.h"

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

		float timeStep = NGN::Application::Get().GetTimestep();
		ImGui::Text("FPS: %.1f", timeStep > 0.0f ? 1.0f / timeStep : 0.0f);

		ImGui::Separator();
		const auto& stats = NGN::Renderer2D::Renderer2DStats::GetStats();

		ImGui::Text("Renderer2D");
		ImGui::Indent();
		ImGui::Text("Draw Calls: %u", stats.DrawCalls);
		ImGui::Text("Quads: %u", stats.QuadCount);
		ImGui::Text("Vertices: %u", stats.GetTotalVertexCount());
		ImGui::Text("Indeces: %u", stats.GetTotalIndexCount());
		ImGui::Unindent();
		if (ImGui::Button("Open Profiling Trace"))
		{
			NGN::Profiling::OpenChromeTracing("logs/ngn_profile.json");
		}

		ImGui::End();
	}
};
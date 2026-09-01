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
		: Layer("Debug")
	{}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");

		float timeStep = NGN::Application::Get().GetTimestep();
		ImGui::Text("FPS: %.1f", timeStep > 0.0f ? 1.0f / timeStep : 0.0f);

		ImGui::Separator();
		const auto& stats = NGN::Renderer2D::GetStats();

		ImGui::Text("Renderer2D");
		ImGui::Indent();
		ImGui::Text("Draw Calls: %u", stats.DrawCalls);
		ImGui::Text("Quads: %u", stats.QuadCount);
		ImGui::Text("Vertices: %u", stats.GetTotalVertexCount());
		ImGui::Text("Indeces: %u", stats.GetTotalIndexCount());
		ImGui::Unindent();

		bool vsync = NGN::Application::Get().GetWindow().IsVSync();
		if (ImGui::Checkbox("VSync", &vsync))
		{
			NGN::Application::Get().GetWindow().SetVSync(vsync);
		}

		if (ImGui::Button("Open Perfetto"))
		{
			NGN::Profiling::OpenChromeTracing("logs/NGNProfile-Runtime.json");
		}

		ImGui::End();

		ImGui::Begin("Profiler");

		const auto& results = NGN::Instrumentor::Get().GetFrameResults();

		for (const auto& result : results)
		{
			float durationMs = (result.End - result.Start) * 0.001f;
			ImGui::Text("%s: %.3f ms", result.Name.c_str(), durationMs);
		}

		ImGui::End();
	}
};
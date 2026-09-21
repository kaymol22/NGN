#pragma once

#include "Core/Layer.h"
#include "Core/Log.h"
#include "Core/Application.h"
#include "Debug/ChromeProfiling.h"
#include "Render/FrameManager.h"

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
		const auto& defaultSubs = NGN::Renderer::FrameManager::GetRenderItemCount(BlendingMode::DEFAULT);
		const auto& alphaDiscSubs = NGN::Renderer::FrameManager::GetRenderItemCount(BlendingMode::ALPHA_DISCARD);
		ImGui::Text("FrameManager RenderItems");
		ImGui::Indent();
		ImGui::Text("Default Submissions: %u", static_cast<int>(defaultSubs));
		ImGui::Text("AlphaDisc Submissions: %u", static_cast<int>(alphaDiscSubs));
		ImGui::Unindent();

		bool vsync = NGN::Application::Get().GetWindow().IsVSync();
		if (ImGui::Checkbox("VSync", &vsync))
		{
			NGN::Application::Get().GetWindow().SetVSync(vsync);
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
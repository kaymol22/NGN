#pragma once
#include "RendererTypes.h"
#include "DrawCommands.h"
#include "Camera.h"
#include "Frustum.h"

namespace NGN::Renderer::FrameManager
{
	void BeginFrame();

	const DrawCommandsSet& GetDrawInfoSet();
	const std::vector<RenderObject>& GetSceneRenderObjects();
	const ViewportData& GetViewportData();
	const RendererData& GetRenderData();
	const std::vector<uint32_t> GetDrawRenderObjectIndices();

	void SubmitRenderObject(const RenderObject renderItem);

	void UpdateViewportData(const Camera& camera);
	void UpdateRendererData();
	void UpdateDrawCommandsSet();
}
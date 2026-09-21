#pragma once
#include "RendererTypes.h"
#include "DrawCommands.h"
#include "Camera.h"
#include "Frustum.h"

namespace NGN::Renderer::FrameManager
{
	void BeginFrame();

	// Submissions
	void SubmitRenderItem(const RenderItem& renderItem);
	void SubmitViewportData(const Camera& camera);

	uint32_t AddSceneRenderItem(const RenderItem& renderItem);
	void AddRenderItemToCategory(std::vector<uint32_t>& categoryIndices, uint32_t renderItemIndex);

	void UpdateDrawCommandsSet();
	void ClearDrawCommandsSet();
	void CreateViewportDrawCommands();
	void CreateDrawCommandsFromIndices(std::vector<DrawIndexedIndirectCommand>& commands, const std::vector<uint32_t>& sortedIndices);

	void SortDrawCommandRenderItems();
	void SortRenderItemIndices(std::vector<uint32_t>& indices);

	const DrawCommandsSet& GetDrawInfoSet();
	const std::vector<RenderItem>& GetSceneRenderItems();
	const ViewportData& GetViewportData();
	const std::vector<uint32_t>& GetDrawRenderItemIndices();
	const std::vector<GPUObjectData>& GetSceneGPUData();
	std::size_t GetRenderItemCount(BlendingMode blendingMode);
}
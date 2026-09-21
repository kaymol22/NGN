#include "FrameManager.h"
#include "RendererEnums.h"
#include "Core/Application.h"
#include "Renderer.h"
#include <glm/matrix.hpp>

namespace NGN::Renderer::FrameManager
{
	namespace {
		DrawCommandsSet g_DrawCommandsSet;
		ViewportData g_ViewportData;
		std::vector<RenderItem> g_SceneRenderItems;
		std::vector<GPUObjectData> g_GPUObjectData;

		std::vector<uint32_t> g_DrawRenderItemIndices;

		std::vector<uint32_t> g_RenderItemIndices;
		std::vector<uint32_t> g_RenderItemIndicesAlphaDiscard;
	}

	void BeginFrame()
	{
		g_SceneRenderItems.clear();
		g_GPUObjectData.clear();
		g_DrawRenderItemIndices.clear();

		g_RenderItemIndices.clear();
		g_RenderItemIndicesAlphaDiscard.clear();
	}

	void SubmitViewportData(const Camera& camera)
	{
		g_ViewportData.viewMatrix = camera.GetViewMatrix();
		g_ViewportData.inverseViewMatrix = glm::inverse(g_ViewportData.viewMatrix);
		g_ViewportData.projectionMatrix = camera.GetProjectionMatrix();
		g_ViewportData.inverseProjectionMatrix = glm::inverse(g_ViewportData.projectionMatrix);
		g_ViewportData.viewProjectionMatrix = g_ViewportData.projectionMatrix * g_ViewportData.viewMatrix;
		g_ViewportData.inverseViewProjectionMatrix = glm::inverse(g_ViewportData.viewProjectionMatrix);
		g_ViewportData.viewPosition = glm::vec4(camera.GetPosition(), 1.0f);
		g_ViewportData.camForward = glm::vec4(camera.GetForward(), 0.0f);
		g_ViewportData.camUp = glm::vec4(camera.GetUp(), 0.0f);
		g_ViewportData.camRight = glm::vec4(camera.GetRight(), 0.0f);
	}

	void SubmitRenderItem(const RenderItem& renderItem)
	{
		BlendingMode blendingMode = renderItem.blendMode;

		if (blendingMode == BlendingMode::DO_NOT_RENDER) return;

		uint32_t renderItemIndex = AddSceneRenderItem(renderItem);

		switch (blendingMode) {
			case BlendingMode::DEFAULT:				AddRenderItemToCategory(g_RenderItemIndices, renderItemIndex); break;
			case BlendingMode::ALPHA_DISCARD:		AddRenderItemToCategory(g_RenderItemIndicesAlphaDiscard, renderItemIndex); break;
			default: break;
		}
	}

	uint32_t AddSceneRenderItem(const RenderItem& renderItem) {
		uint32_t renderItemIndex = static_cast<uint32_t>(g_SceneRenderItems.size());
		g_SceneRenderItems.push_back(renderItem);
		g_GPUObjectData.push_back(renderItem.gpuData);
		return renderItemIndex;
	}

	void AddRenderItemToCategory(std::vector<uint32_t>& categoryIndices, uint32_t renderItemIndex) {
		categoryIndices.push_back(renderItemIndex);
	}

	// TODO: Add different sort method for blended
	void SortRenderItemIndices(std::vector<uint32_t>& indices)
	{
		std::sort(indices.begin(), indices.end(), [](uint32_t a, uint32_t b) {
			return g_SceneRenderItems[a].meshId < g_SceneRenderItems[b].meshId;
		});
	}

	void CreateDrawCommandsFromIndices(std::vector<DrawIndexedIndirectCommand>& commands, const std::vector<uint32_t>& sortedIndices)
	{
		MeshBuffer& assetBuffer = GetMeshBuffer("AssetGeometry");

		size_t i = 0;
		while (i < sortedIndices.size()) {
			uint32_t renderItemIndex = sortedIndices[i];
			uint32_t meshId = g_SceneRenderItems[renderItemIndex].meshId;
			uint32_t baseInstance = static_cast<uint32_t>(g_DrawRenderItemIndices.size());

			// Create run of items with shared meshId - increment instancecount & avoid submitting duplicates
			size_t runStart = i;
			while (i < sortedIndices.size() && g_SceneRenderItems[sortedIndices[i]].meshId == meshId) {
				g_DrawRenderItemIndices.push_back(sortedIndices[i]);
				++i;
			}

			const MeshDescriptor& mesh = assetBuffer.GetMesh(meshId);
			commands.push_back(DrawIndexedIndirectCommand{
				mesh.indexCount,
				static_cast<uint32_t>(i - runStart),
				mesh.baseIndex,
				static_cast<int32_t>(mesh.baseVertex),
				baseInstance
			});
		}
	}

	void UpdateDrawCommandsSet()
	{
		NGN_PROFILE_FUNCTION();

		ClearDrawCommandsSet();
		SortDrawCommandRenderItems();

		CreateViewportDrawCommands();
	}

	void ClearDrawCommandsSet()
	{
		g_DrawCommandsSet.standard.clear();
		g_DrawCommandsSet.alphaDiscard.clear();
	}

	void SortDrawCommandRenderItems()
	{
		SortRenderItemIndices(g_RenderItemIndices);
		SortRenderItemIndices(g_RenderItemIndicesAlphaDiscard);
	}

	void CreateViewportDrawCommands()
	{
		NGN_PROFILE_FUNCTION();

		CreateDrawCommandsFromIndices(g_DrawCommandsSet.standard, g_RenderItemIndices);
		CreateDrawCommandsFromIndices(g_DrawCommandsSet.alphaDiscard, g_RenderItemIndicesAlphaDiscard);
	}

	std::size_t GetRenderItemCount(BlendingMode blendingMode)
	{
		switch (blendingMode)
		{
			case BlendingMode::DEFAULT:					return g_RenderItemIndices.size();
			case BlendingMode::ALPHA_DISCARD:			return g_RenderItemIndicesAlphaDiscard.size();
			default: return 0;
		}
	}

	// Getters
	const ViewportData& GetViewportData() { return g_ViewportData; }
	const DrawCommandsSet& GetDrawInfoSet() { return g_DrawCommandsSet; }
	const std::vector<RenderItem>& GetSceneRenderItems() { return g_SceneRenderItems; }
	const std::vector<uint32_t>& GetDrawRenderItemIndices() { return g_DrawRenderItemIndices; }
	const std::vector<GPUObjectData>& GetSceneGPUData() { return g_GPUObjectData; }
}
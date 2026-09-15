#include "FrameManager.h"
#include "Camera.h"
#include "Core/Application.h"
#include <glm/matrix.hpp>

namespace NGN::Renderer::FrameManager
{

	ViewportData g_ViewportData;
	RendererData g_RendererData;
	std::vector<RenderObject> g_SceneRenderObjects;
	std::vector<uint32_t> g_SceneRenderItemIndices;

	void BeginFrame()
	{
		g_SceneRenderObjects.clear();
		g_SceneRenderItemIndices.clear();

	}

	void UpdateViewportData(const Camera& camera)
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

	const RendererData& GetRenderData()
	{
		return g_RendererData;
	}

	void SubmitRenderObject(const RenderObject renderItem)
	{
		g_SceneRenderObjects.push_back(renderItem);
	}
}
#pragma once
#include "RendererTypes.h"

namespace NGN::Renderer::DataManager
{
	const RendererData& GetRenderData();
	void SubmitRenderItem(const RenderItem renderItem);
	void SubmitRenderItems(const std::vector<RenderItem>& renderItems);
}
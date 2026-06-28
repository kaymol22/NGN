#pragma once

#include "RenderPass.h"
#include "Core/Types.h"

namespace NGN
{
	class RenderContext
	{
	public:
		// Pass management
		RenderPass& GetPass(std::string_view name);

		// Pass Config
		void SetPassTarget(std::string_view name, Ref<Framebuffer> target);
		void SetPassOrder(const std::vector<std::string>& order);

		void Submit(const RenderItem& item, std::string_view passName);
		void Execute();

	private:
		std::unordered_map<std::string, RenderPass> m_RenderPasses;
		std::vector<std::string> m_ExecutionOrder;
	};
}
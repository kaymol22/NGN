#pragma once

#include "Resources/Framebuffer.h"
#include "Core/Types.h"

namespace NGN
{
	class RenderPass
	{
	public:
		virtual ~RenderPass() = default;
		
		void Submit(RenderItem&& item) { m_Items.push_back(std::move(item)); }
		void Clear() { m_Items.clear(); }
		void Execute(Ref<Framebuffer> target);

		int GetItemCount() const { return static_cast<int>(m_Items.size()); }

	protected:
		virtual void Render(Ref<Framebuffer> target) = 0;
		std::vector<RenderItem> m_Items;
	};
}
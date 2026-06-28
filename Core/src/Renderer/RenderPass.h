#pragma once

#include "Resources/Framebuffer.h"
#include "Core/Types.h"

namespace NGN
{
	class RenderPass
	{
	public:
		
		void SetTarget(const Ref<Framebuffer>& target) { m_Target = target; }
		const Ref<Framebuffer>& GetTarget() const { return m_Target; }

		void Submit(const RenderItem& item);
		void Clear();
		void Execute();
	private:
		Ref<Framebuffer> m_Target;
		std::vector<RenderItem> m_Items;
	};
}
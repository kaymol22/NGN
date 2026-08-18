#include "RenderPass.h"

namespace NGN
{
	void RenderPass::Execute(Ref<Framebuffer> target)
	{
		NGN_PROFILE_FUNCTION();
		if (m_Items.empty())
		{
			NGN_CORE_WARN("RenderPass::Execute - No items to render");
			return;
		}
		Render(target);
	}
}
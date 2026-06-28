#include "ngnpch.h"
#include "RenderContext.h"

namespace NGN
{
	/*RenderPass& RenderContext::GetPass(std::string_view name)
	{
		return  ;
	}*/

	void RenderContext::SetPassTarget(std::string_view name, Ref<Framebuffer> target)
	{

	}

	void RenderContext::SetPassOrder(const std::vector<std::string>& order)
	{

	}

	void RenderContext::Submit(const RenderItem& item, std::string_view passName)
	{

	}

	void RenderContext::Execute()
	{

	}
}
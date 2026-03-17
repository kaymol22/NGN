#include "RenderCommand.h"

namespace NGN
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = nullptr;

	void RenderCommand::Init()
	{
		s_RendererAPI = RendererAPI::Create();
		s_RendererAPI->Init();
	}
}
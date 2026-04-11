#include <Core/EntryPoint.h>

#include "GameLayer.h"
#include "DebugLayer.h"

class Sandbox : public NGN::Application
{
public:

	Sandbox(const NGN::ApplicationSpecification& spec) : NGN::Application(spec)
	{
		NGN_INFO("Creating Sandbox Application");
		PushLayer<GameLayer>();
		/*PushLayer<DebugLayer>();*/
	}

	~Sandbox()
	{
	}
};

NGN::Application* NGN::CreateApplication(NGN::ApplicationCmdLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "NGN Sandbox";
	spec.RendererAPI = NGN::RendererAPIType::OpenGL;
	spec.WindowSpec.Title = "NGN Sandbox";
	spec.WindowSpec.Width = 1280;
	spec.WindowSpec.Height = 720;
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}

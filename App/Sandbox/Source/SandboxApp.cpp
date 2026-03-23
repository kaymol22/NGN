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
		PushLayer<DebugLayer>();
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
	spec.WindowSpec.Width = 1000;
	spec.WindowSpec.Height = 1000;
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}

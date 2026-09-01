#include <Core/EntryPoint.h>

#include "BackendTestLayer.h"

class Sandbox : public NGN::Application
{
public:

	Sandbox(const NGN::ApplicationSpecification& spec) : NGN::Application(spec)
	{
		NGN_INFO("Creating Sandbox Application");
		PushLayer<BackendTestLayer>();
	}

	~Sandbox()
	{
	}
};

NGN::Application* NGN::CreateApplication(NGN::ApplicationCmdLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "NGN Sandbox";
	spec.APIspec = API::OPENGL;
	spec.WindowSpec.Title = "NGN Sandbox";
	spec.WindowSpec.Width = 1280;
	spec.WindowSpec.Height = 720;
	spec.WindowSpec.VSync = true;
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}

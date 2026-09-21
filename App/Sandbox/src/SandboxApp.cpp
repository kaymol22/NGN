#include <Core/EntryPoint.h>

#include "BackendTestLayer.h"
#include "DebugLayer.h"

class Sandbox : public NGN::Application
{
public:

	Sandbox(const NGN::ApplicationSpecification& spec) : NGN::Application(spec)
	{
		PushLayer<BackendTestLayer>();
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
	spec.APIspec = API::OPENGL;
	spec.WindowSpec.Title = "NGN Sandbox";
	spec.WindowSpec.Width = 1280;
	spec.WindowSpec.Height = 720;
	spec.WindowSpec.VSync = true;
	spec.Resolutions.gBuffer = { 1280, 720 };
	spec.Resolutions.finalImage = { 1280 / 2, 720 / 2};
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}

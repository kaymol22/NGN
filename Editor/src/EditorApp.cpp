#include <NGN.h>
#include "Core/EntryPoint.h"
#include "EditorLayer.h"

namespace NGN
{
	class EditorApp : public NGN::Application
	{
	public:
		EditorApp(const ApplicationSpecification& appSpec) : Application(appSpec)
		{
			PushLayer<EditorLayer>();
		}

		~EditorApp()
		{
		}
	};

	Application* CreateApplication(ApplicationCmdLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "NGN Editor";
		spec.RendererAPI = NGN::RendererAPIType::OpenGL;
		spec.CommandLineArgs = args;

		return new EditorApp(spec);
	}
}
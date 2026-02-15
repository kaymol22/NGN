//#include <NGN.h>
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

		Application* CreateApplication(ApplicationCmdLineArgs args)
		{
			ApplicationSpecification spec;
			spec.Name = "Editor";
			spec.CommandLineArgs = args;

			return new EditorApp(spec);
		}
	};
}
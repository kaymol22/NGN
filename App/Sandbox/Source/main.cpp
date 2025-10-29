#include "Core/Application.h"

#include "AppLayer.h"
#include "OverlayLayer.h"

int main() {
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Layer Test";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 600;

	Core::Application application(appSpec);
	application.PushLayer<AppLayer>();
	/*application.PushLayer<OverlayLayer>();*/
	application.Run();
}
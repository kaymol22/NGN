#include "Core/Application.h"

#include "AppLayer.h"
#include "OverlayLayer.h"

int main() {
	NGN::ApplicationSpecification appSpec;
	appSpec.Name = "NGN Layer Test";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 600;

	NGN::Application application(appSpec);
	application.PushLayer<AppLayer>();
	/*application.PushLayer<OverlayLayer>();*/
	application.Run();
}
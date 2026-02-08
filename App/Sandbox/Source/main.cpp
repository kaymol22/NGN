#include "Core/Application.h"

#include "AppLayer.h"
#include "OverlayLayer.h"
#include "GameLayer.h"
#include "DebugLayer.h"
#include "Core/Log.h"

int main() {
	NGN::Log::Init();

	NGN::ApplicationSpecification appSpec;
	appSpec.Name = "NGN Layer Test";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 800;

	NGN::Application application(appSpec);
	/*application.PushLayer<AppLayer>();*/
	/*application.PushLayer<OverlayLayer>(); */
	application.PushLayer<GameLayer>();
	application.PushLayer<DebugLayer>();
	application.Run();
}
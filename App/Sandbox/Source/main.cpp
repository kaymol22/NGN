#include "Core/Application.h"

#include "AppLayer.h"
#include "OverlayLayer.h"
#include "Core/Log.h"

int main() {
	NGN::Log::Init();

	NGN::ApplicationSpecification appSpec;
	appSpec.Name = "NGN Layer Test";
	appSpec.WindowSpec.Width = 800;
	appSpec.WindowSpec.Height = 600;

	int a = 5;
	NGN_CORE_WARN("Core Log Test");
	NGN_TRACE("Test var include Var={0}", a);

	NGN::Application application(appSpec);
	application.PushLayer<AppLayer>();
	/*application.PushLayer<OverlayLayer>();*/
	application.Run();
}
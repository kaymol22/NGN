#pragma once

#include "Core/Base.h"
#include "Core/Application.h"
#include "Core/Profile.h"

#ifdef NGN_PLATFORM_WINDOWS

extern NGN::Application* NGN::CreateApplication(NGN::ApplicationCmdLineArgs args);

int main(int argc, char** argv)
{
	NGN::Log::Init();

	NGN_PROFILE_BEGIN_SESSION("AppStart", "logs/NGNProfile-Startup.json");
	auto app = NGN::CreateApplication({ argc, argv });
	NGN_PROFILE_END_SESSION();

	NGN_PROFILE_BEGIN_SESSION("Runtime", "logs/NGNProfile-Runtime.json");
	app->Run();
	NGN_PROFILE_END_SESSION();

	NGN_PROFILE_BEGIN_SESSION("Shutdown", "logs/NGNProfile-Shutdown.json");
	delete app;
	NGN_PROFILE_END_SESSION();
}

#endif
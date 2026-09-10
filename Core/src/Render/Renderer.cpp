#include "Renderer.h"
#include "API/OpenGL/GL_Renderer.h"
#include "Core/Application.h"

namespace NGN
{
	void Renderer::RenderScene()
	{
		if (Application::Get().GetAPI() == API::OPENGL)
		{
			
		}
		else
		{
			NGN_CORE_ERROR("Renderer::RenderScene - Unsupported graphics API");
		}
	}
}
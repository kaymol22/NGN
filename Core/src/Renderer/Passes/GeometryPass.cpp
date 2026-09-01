#include "GeometryPass.h"
#include "Renderer/Backend/RendererAPI.h"
#include "Platform/OpenGL/Passes/GL_GeometryPass.h"

namespace NGN
{
	Scope<GeometryPass> GeometryPass::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case API::OPENGL:
				return CreateScope<OpenGLGeometryPass>();

			case API::UNDEFINED:
				NGN_CORE_ASSERT(false, "GRAPHICS API UNDEFINED - Cannot create geometry pass"); 
				return nullptr;
		}
	}
}
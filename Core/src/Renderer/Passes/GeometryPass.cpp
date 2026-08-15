#include "GeometryPass.h"
#include "Renderer/Backend/RendererAPI.h"
#include "Platform/OpenGL/Passes/GL_GeometryPass.h"

namespace NGN
{
	Scope<GeometryPass> GeometryPass::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPIType::OPENGL: 
				return CreateScope<OpenGLGeometryPass>();

			case RendererAPIType::UNDEFINED: 
				NGN_CORE_ASSERT(false, "GRAPHICS API UNDEFINED - Cannot create geometry pass"); 
				return nullptr;
		}
	}
}
#include "Renderer.h"
#include "API/OpenGL/GL_Renderer.h"

namespace NGN::Renderer
{
	namespace {
		API g_ActiveAPI = API::UNDEFINED;
		std::unordered_map<std::string, MeshBuffer> g_MeshBuffers;
	}

	API GetActiveAPI() { return g_ActiveAPI; }

	void Init(API api)
	{
		g_ActiveAPI = api;

		if (g_ActiveAPI == API::OPENGL) {
			OpenGL::ResourceManager::Init();
		}

		g_MeshBuffers.emplace("AssetGeometry", MeshBuffer("AssetGeometry"));

		for (auto& [name, buffer] : g_MeshBuffers) {
			buffer.Initialize();
		}

		if (g_ActiveAPI == API::OPENGL) {
			OpenGL::Renderer::Init();
		}
	}

	void Renderer::RenderScene()
	{
		if (g_ActiveAPI == API::OPENGL)
		{
			OpenGL::Renderer::RenderDeferred();
		}
		else
		{
			NGN_CORE_ERROR("Renderer::RenderScene - Unsupported graphics API");
		}
	}

	void Shutdown()
	{
		if (g_ActiveAPI == API::OPENGL) {
			OpenGL::Renderer::Shutdown();
		}
		g_MeshBuffers.clear();

		if (g_ActiveAPI == API::OPENGL) {
			OpenGL::ResourceManager::CleanUp();
		}
	}

	MeshBuffer& GetMeshBuffer(const std::string& name) {
		auto it = g_MeshBuffers.find(name);
		NGN_CORE_ASSERT(it != g_MeshBuffers.end(), "MeshBuffer '{}' not found", name);
		return it->second;
	}
}
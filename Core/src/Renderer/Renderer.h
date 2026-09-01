#pragma once

#include "Backend/RenderCommand.h"
#include "RenderPass.h"
#include "RenderGraph.h"

#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/SubTexture.h"
#include "Resources/Mesh.h"
#include "Resources/Camera.h"
#include "Resources/Frustum.h"

namespace NGN
{
	class SceneCamera;

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		// Frame lifecycle
		static void BeginFrame();
		static void Flush();  // Execute all render passes
		static void EndFrame();

		// Camera (scene culling) - set before renderitem submission
		static void SetCamera(const SceneCamera& camera);
		static SceneCamera* GetCurrentCamera() { return s_CurrentCamera; }

		// Access render data for culling in systems - could store more useful info here
		struct SceneRenderData
		{
			const Frustum& frustum;
			const glm::vec3& cameraPosition;
		};

		static SceneRenderData GetSceneData();

		static void Submit(RenderItem&& item);

		static void SetFramebuffer(std::string_view name, Ref<Framebuffer> framebuffer);
		static Ref<Framebuffer> GetFramebuffer(std::string_view name);

		static void OnWindowResize(uint32_t width, uint32_t height);
		static API GetAPI() { return RendererAPI::GetAPI(); }

		static RenderGraph& GetRenderGraph()
		{
			NGN_CORE_ASSERT(s_RenderGraph, "RenderGraph not intialised");
			return *s_RenderGraph;
		}

		/*template <typename TPass>
			requires std::derived_from<TPass, RenderPass>
		static TPass* GetPass(PassType type)
		{
			NGN_CORE_ASSERT(s_Context, "RenderContext not initialised, could not get Pass");
			return s_Context->GetPass<TPass>(type);
		}*/
		
	private:
		struct SceneData
		{
			Frustum frustum;
			glm::vec3 camPosition;
			glm::vec3 camForward;
		};

		static Scope<RenderGraph> s_RenderGraph;
		/*static Scope<RenderContext> s_Context;*/
		static Scope<SceneData> s_SceneData; // Needs to be shared for renderpasses to access
		static class SceneCamera* s_CurrentCamera;
	};
}

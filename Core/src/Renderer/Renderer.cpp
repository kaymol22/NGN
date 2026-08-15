#include "ngnpch.h"
#include "Renderer.h"
//#include "Renderer2D.h"
#include "Scene/SceneCamera.h"

namespace NGN
{
	/*Scope<RenderContext> Renderer::s_Context;*/
	Scope<RenderGraph> Renderer::s_RenderGraph;
	Scope<Renderer::SceneData> Renderer::s_SceneData;
	SceneCamera* Renderer::s_CurrentCamera = nullptr;

	void Renderer::Init()
	{
		NGN_PROFILE_FUNCTION();

		RenderCommand::Init();
		s_SceneData = CreateScope<SceneData>();
		/*s_Context = CreateScope<RenderContext>();*/

		s_RenderGraph = CreateScope<RenderGraph>();
		s_RenderGraph->Build();

		NGN_CORE_INFO("Renderer Initialized");
	}

	void Renderer::Shutdown()
	{
		NGN_PROFILE_FUNCTION();
		/*s_Context = nullptr;*/
		s_RenderGraph = nullptr;
		s_SceneData = nullptr;
		NGN_CORE_INFO("Renderer Shutdown");
	}

	void Renderer::BeginFrame()
	{
		NGN_PROFILE_FUNCTION();
		RenderCommand::Clear();
		s_RenderGraph->BeginFrame();
	}

	void Renderer::SetCamera(const SceneCamera& camera)
	{
		NGN_PROFILE_FUNCTION();
		NGN_CORE_ASSERT(s_SceneData, "Scene data not initialised");

		s_CurrentCamera = const_cast<SceneCamera*>(&camera);

		// Update frustum & cam pos
		s_SceneData->frustum.Update(camera.GetViewProjectionMatrix());
		s_SceneData->camPosition = camera.GetPosition();
		s_SceneData->camForward = camera.GetForward();
	}

	Renderer::SceneRenderData Renderer::GetSceneData()
	{
		NGN_CORE_ASSERT(s_SceneData, "Scene data not initialised");
		return SceneRenderData{
			s_SceneData->frustum,
			s_SceneData->camPosition
		};
	}

	void Renderer::Submit(const RenderItem& item)
	{
		NGN_PROFILE_FUNCTION();
		NGN_CORE_ASSERT(s_RenderGraph, "RenderGraph not initialised");
		s_RenderGraph->Submit(item);
	}

	void Renderer::Flush()
	{
		NGN_PROFILE_FUNCTION();
		NGN_CORE_ASSERT(s_RenderGraph, "RenderGraph not initialised");
		s_RenderGraph->Execute();
	}

	void Renderer::EndFrame()
	{
		NGN_PROFILE_FUNCTION();
		s_CurrentCamera = nullptr;
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		NGN_PROFILE_FUNCTION();

		RenderCommand::SetViewport(0, 0, width, height);
	}
}

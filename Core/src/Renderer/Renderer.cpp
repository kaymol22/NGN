#include "ngnpch.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace NGN
{
	Scope<Renderer::SceneData> Renderer::s_SceneData;

	void Renderer::Init()
	{
		NGN_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
		s_SceneData = CreateScope<SceneData>();
	}

	void Renderer::Shutdown()
	{
		NGN_PROFILE_FUNCTION();
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		NGN_PROFILE_FUNCTION();

		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		NGN_PROFILE_FUNCTION();

		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		NGN_PROFILE_FUNCTION();
		NGN_CORE_INFO("Renderer::EndScene called");
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
#include "ngnpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Scene/SceneCamera.h"

namespace NGN
{
	Scope<Renderer::SceneData> Renderer::s_SceneData;
	SceneCamera* Renderer::s_CurrentCamera = nullptr;

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

	void Renderer::BeginFrame()
	{
		NGN_PROFILE_FUNCTION();
		// Setup will happen when SetCamera is called
	}

	void Renderer::SetCamera(const SceneCamera& camera)
	{
		NGN_PROFILE_FUNCTION();
		s_CurrentCamera = const_cast<SceneCamera*>(&camera);

		// Update render data
		s_SceneData->frustum.Update(camera.GetViewProjectionMatrix());
		s_SceneData->camPosition = camera.GetPosition();

		// Initialize Renderer2D when camera is set
		Renderer2D::BeginScene(camera);
	}

	Renderer::SceneRenderData Renderer::GetSceneData()
	{
		NGN_CORE_ASSERT(s_SceneData, "Scene data not initialized!");
		return SceneRenderData{
			s_SceneData->frustum,
			s_SceneData->camPosition
		};
	}

	void Renderer::Submit2D(
		const glm::mat4& transform,
		const glm::vec4& color,
		int entityID)
	{
		NGN_CORE_ASSERT(s_CurrentCamera, "Camera not set! Call Renderer::SetCamera before submitting");
		Renderer2D::DrawQuad(transform, color, entityID);
	}

	void Renderer::Submit2D(
		const glm::mat4& transform,
		const Ref<Texture2D>& texture,
		float tilingFactor,
		const glm::vec4& tintColor,
		int entityID)
	{
		NGN_CORE_ASSERT(s_CurrentCamera, "Camera not set! Call Renderer::SetCamera before submitting");
		Renderer2D::DrawQuad(transform, texture, tilingFactor, tintColor, entityID);
	}

	void Renderer::Submit2D(
		const glm::mat4& transform,
		const Ref<SubTexture2D>& subtexture,
		float tilingFactor,
		const glm::vec4& tintColor,
		int entityID)
	{
		NGN_CORE_ASSERT(s_CurrentCamera, "Camera not set! Call Renderer::SetCamera before submitting");
		// For subtextures, we need to extract position/scale from transform and use position-based API
		// This is a temporary solution - ideally Renderer2D would have mat4-based subtexture support
		glm::vec3 translation = glm::vec3(transform[3]);
		glm::vec3 scale = glm::vec3(
			glm::length(glm::vec3(transform[0])),
			glm::length(glm::vec3(transform[1])),
			glm::length(glm::vec3(transform[2]))
		);

		Renderer2D::DrawQuad(
			translation,
			glm::vec2(scale.x, scale.y),
			subtexture,
			tilingFactor,
			tintColor,
			entityID
		);
	}

	void Renderer::Flush()
	{
		NGN_PROFILE_FUNCTION();
		NGN_CORE_ASSERT(s_CurrentCamera, "Camera not set! Call Renderer::SetCamera before flushing");

		// EndScene calls Flush internally, which batches and renders all queued data
		Renderer2D::EndScene();
	}

	void Renderer::EndFrame()
	{
		NGN_PROFILE_FUNCTION();
		s_CurrentCamera = nullptr;
	}

	// Legacy interface - kept for compatibility
	void Renderer::BeginScene(Camera& camera)
	{
		NGN_PROFILE_FUNCTION();
	}

	void Renderer::EndScene()
	{
		NGN_PROFILE_FUNCTION();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		NGN_PROFILE_FUNCTION();

		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_CurrentCamera->GetViewProjectionMatrix());
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}

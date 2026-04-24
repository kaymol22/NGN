#pragma once
#pragma once

#include "RenderCommand.h"
#include "RenderPass.h"

#include "Shader.h"
#include "Texture.h"
#include "SubTexture.h"
#include "Mesh.h"
#include "Camera.h"
#include "Frustum.h"

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
		static void Flush();  // Execute all rendering passes
		static void EndFrame();

		// Camera management - provides render data to systems
		static void SetCamera(const SceneCamera& camera);

		// Access render data for culling in systems
		struct SceneRenderData
		{
			const Frustum& frustum;
			const glm::vec3& cameraPosition;
		};

		static SceneRenderData GetSceneData();

		// 2D Submission
		static void Submit2D(
			const glm::mat4& transform,
			const glm::vec4& color,
			int entityID = -1
		);

		static void Submit2D(
			const glm::mat4& transform,
			const Ref<Texture2D>& texture,
			float tilingFactor = 1.0f,
			const glm::vec4& tintColor = glm::vec4(1.0f),
			int entityID = -1
		);

		static void Submit2D(
			const glm::mat4& transform,
			const Ref<SubTexture2D>& subtexture,
			float tilingFactor = 1.0f,
			const glm::vec4& tintColor = glm::vec4(1.0f),
			int entityID = -1
		);

		// 3D Submission
		static void Submit3D(
			const glm::mat4& transform,
			const Ref<Mesh>& mesh,
			const Ref<Shader>& shader,
			const glm::vec4& color = glm::vec4(1.0f),
			int entityID = -1
		);

		// Legacy 3D interface (for now)
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static RendererAPIType GetAPI() { return RendererAPI::GetAPI(); }
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

	private:
		struct SceneData
		{
			Frustum frustum;
			glm::vec3 camPosition;
			glm::vec3 camForward;
		};

		static Scope<SceneData> s_SceneData; // Needs to be shared for renderpasses to access
		static class SceneCamera* s_CurrentCamera;
	};
}

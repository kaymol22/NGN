#pragma once

#include "OrthographicCamera.h"
#include "Camera.h"
#include "Texture.h"

namespace NGN
{
	using EntityID = uint32_t;

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		/* For scene camera's/entities*/
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		/* For editor/debug */
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const glm::mat4& vpMatrix);

		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, EntityID id);

		static void Flush();

		struct Renderer2DStats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t LineCount = 0;
			uint32_t CircleCount = 0;

			uint32_t GetTotalVertexCount() const {
				return QuadCount * 4
					+ LineCount * 2
					+ CircleCount * 4;
			}

			uint32_t GetTotalIndexCount() const {
				return QuadCount * 6
					+ LineCount * 2
					+ CircleCount * 6;
			}

			static void ResetStats();
			static Renderer2DStats GetStats();
		};
	private:
		static void StartBatch();
		static void NextBatch();
	};
}
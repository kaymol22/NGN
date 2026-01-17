#pragma once

#include "OrthographicCamera.h"
#include "Camera.h"
#include "Texture.h"

namespace NGN
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		/* For scene camera's/entities*/
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		/* For editor/debug */
		static void BeginScene(const OrthographicCamera& camera);

		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void DrawQuad(const::glm::mat4& transform, const glm::vec4& color);
	};
}
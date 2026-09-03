#include "GL_Renderer.h"
#include "GL_Backend.h"

#include "Render/SpriteExtraction.h"
#include "Core/Application.h"
#include "Core/GraphicsContext.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace OpenGL::Renderer
{
	void RenderScene()
	{
		NGN::Scene* scene = NGN::Application::Get().GetSceneManager().GetActiveScene();
		if (!scene) {
			NGN_CORE_INFO("No active scene found");
			return;
		}

		std::vector<NGN::SpriteRenderItem> sprites = NGN::ExtractVisibleSprites(*scene);
		if (sprites.empty()) {
			NGN_CORE_INFO("No sprites found in scene");
		}
		else {
			NGN_CORE_INFO("'{}' Sprites added to RenderScene", sprites.size());
		}

		// Create GPU side textures, upload, draw etc.
	}
}
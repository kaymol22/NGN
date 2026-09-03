#include "SpriteExtraction.h"
#include "Frustum.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"

namespace NGN
{
	std::vector<SpriteRenderItem> ExtractVisibleSprites(Scene& scene)
	{
		NGN_PROFILE_SCOPE("SpriteExtract");
		std::vector<SpriteRenderItem> items;

		Entity camEntity = scene.GetPrimaryCamera();
		if (!camEntity.IsValid())
		{
			NGN_CORE_INFO("ExtractViusibleSprites - no primary camera found");
			return items;
		}

		auto& camComp = camEntity.GetComponent<CameraComponent>();
		auto& camTransform = camEntity.GetComponent<TransformComponent>();
		camComp.Camera.RecalculateViewMatrix(camTransform.Translation, camTransform.Rotation);

		Frustum frustum;
		frustum.Update(camComp.Camera.GetViewProjectionMatrix());

		entt::registry& registry = scene.GetRegistry();
		auto view = registry.view<TransformComponent, SpriteComponent>();

		items.reserve(view.size());

		for (auto handle : view)
		{
			auto& sprite = view.get<SpriteComponent>(handle);
			auto& transform = view.get<TransformComponent>(handle);

			if (!sprite.Texture.IsValid())
			{
				NGN_CORE_INFO("SpriteTexture not valid, skipping");
				continue;
			}

			sprite.UpdateBounds(transform);

			if (!frustum.ContainsAABB(sprite.boundsMin, sprite.boundsMax))
			{
				NGN_CORE_INFO("Sprite not within frustum, skipping");
				continue;
			}

			SpriteRenderItem item;
			item.transform = transform.GetTransform();
			item.Texture = sprite.Texture;
			item.Color = sprite.Color;
			item.TilingFactor = sprite.TilingFactor;

			items.push_back(std::move(item));
			NGN_CORE_INFO("SpriteRenderItem built");
		}

		return items;
	}
}
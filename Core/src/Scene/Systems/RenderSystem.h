#pragma once

#include "System.h"
#include "Scene/Scenebvh.h"
#include "Scene/Entity.h"


namespace NGN
{
	class RenderSystem : public System
	{
	public:
		virtual void OnCreate(Scene& scene) override;
		virtual void OnUpdate(Scene& scene, Timestep ts) override;

	private:
		void UpdateCameraFrustum(Scene& scene);
		void UpdateVisibleEntities(Scene& scene);
		RenderItem BuildRenderItem(Entity entity);

	private:
		SceneBVH m_BVH;
		bool m_BVHDirty = true;
		std::vector<entt::entity> m_VisibleEntities;
		Frustum m_Frustum;
	};
}
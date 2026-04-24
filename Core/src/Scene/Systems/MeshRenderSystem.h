#pragma once

#include "Scene/Scene.h"
#include "System.h"
#include "Scene/Components.h"
#include "Scene/SceneCamera.h"

namespace NGN
{
    class MeshRenderSystem : public System
    {
    public:
        void OnCreate(Scene& scene) override;
        void OnUpdate(Scene& scene, Timestep ts) override;
        void OnRender(Scene& scene, const SceneCamera& camera, const void* renderData) override;
    };
}

#include "MeshRenderSystem.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer.h"

namespace NGN
{
    void MeshRenderSystem::OnCreate(Scene& scene)
    {
        NGN_PROFILE_FUNCTION();
    }

    void MeshRenderSystem::OnRender(Scene& scene, const SceneCamera& camera, const void* renderData)
    {
        NGN_PROFILE_FUNCTION();

        auto& registry = scene.GetRegistry();
        auto view = registry.view<TransformComponent, MeshComponent>();

        // Get render data for culling
        const Renderer::SceneRenderData* renderDataPtr = reinterpret_cast<const Renderer::SceneRenderData*>(renderData);

        for (auto entity : view)
        {
            auto& transform = registry.get<TransformComponent>(entity);
            auto& meshComp = registry.get<MeshComponent>(entity);
            auto id = registry.get<IDComponent>(entity);

            if (!meshComp.MeshRef || !meshComp.ShaderRef)
                continue;

            // Update mesh bounds based on current transform
            meshComp.UpdateBounds(transform);

            // Early frustum culling
            if (renderDataPtr)
            {
                if (!renderDataPtr->frustum.ContainsAABB(meshComp.boundsMin, meshComp.boundsMax))
                    continue;
            }

            // Submit 3D mesh
            Renderer::Submit3D(
                transform.GetTransform(),
                meshComp.MeshRef,
                meshComp.ShaderRef,
                meshComp.Color,
                id.ID
            );
        }
    }

    void MeshRenderSystem::OnUpdate(Scene& scene, Timestep ts)
    {
        NGN_PROFILE_FUNCTION();
    }
}

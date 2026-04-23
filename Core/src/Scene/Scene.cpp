#include "ngnpch.h"
#include "Scene.h"
#include "Components.h"

#include "Renderer/PerspectiveCamera.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/Renderer.h"
#include "SceneCamera.h"

#include "Systems/SpriteRenderSystem.h"
#include "Systems/PlayerControllerSystem.h"

namespace NGN
{
	Scene::Scene()
	{
		NGN_CORE_INFO("Scene Created");

		AddSystem<SpriteRenderSystem>();
		/*AddSystem<PlayerControllerSystem>();*/
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity.GetHandle();
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		NGN_CORE_ASSERT(entity.IsValid(), "Cannot destroy invalid entity");

		// Remove ID component from UUID map 
		UUID id = entity.GetComponent<IDComponent>().ID;
		m_EntityMap.erase(id);

		m_Registry.destroy(entity.GetHandle());
	}

	Entity Scene::GetEntityByUUID(UUID id)
	{
		auto target = m_EntityMap.find(id);
		if (target != m_EntityMap.end())
		{
			return Entity(target->second, this);
		}
		return Entity(); // Return invalid entity if not found
	}

	bool Scene::EntityExists(UUID id) const
	{
		return m_EntityMap.find(id) != m_EntityMap.end();
	}

	Entity Scene::GetPrimaryCamera()
	{
		auto cameras = GetEntitiesWithComponents<CameraComponent>();
		for (auto camera : cameras)
		{
			if (camera.GetComponent<CameraComponent>().Primary)
				return camera;
		}
		if (!cameras.empty())
			return cameras[0];

		return Entity(); // Return invalid entity if no cameras found
	}

	void Scene::OnUpdate(Timestep ts)
	{
		for (auto& system : m_Systems)
			system->OnUpdate(*this, ts);
	}

	void Scene::RenderScene()
	{
		NGN_PROFILE_FUNCTION();

		Entity cameraEntity = GetPrimaryCamera();
		if (!cameraEntity.IsValid())
			return;

		auto& cameraComp = cameraEntity.GetComponent<CameraComponent>();
		auto& cameraTransform = cameraEntity.GetComponent<TransformComponent>();

		cameraComp.Camera.RecalculateViewMatrix(cameraTransform.Translation, cameraTransform.Rotation);

		// Set camera globally for the frame
		Renderer::SetCamera(cameraComp.Camera);

		// Get render data for passing to systems
		const Renderer::SceneRenderData renderData = Renderer::GetSceneData();

		// Systems submit culled renderable scene back to renderer
		for (auto& system : m_Systems)
			system->OnRender(*this, cameraComp.Camera, (const void*)&renderData);
	}
}
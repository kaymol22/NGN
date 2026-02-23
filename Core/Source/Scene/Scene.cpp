#include "Core/ngnpch.h"
#include "Scene.h"
#include "Systems/System.h"
#include "Components.h"

namespace NGN
{
	Scene::Scene()
	{ 
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

	void Scene::OnUpdate(Timestep ts)
	{
		for (auto& system : m_Systems)
			system->OnUpdate(*this, ts);
	}

	void Scene::OnRender()
	{
		for (auto& system : m_Systems)
			system->OnRender(*this);
	}
}
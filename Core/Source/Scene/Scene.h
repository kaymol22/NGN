#pragma once

#include "World.h"
#include "System.h"
#include "Entity.h"

namespace NGN
{
	class Scene 
	{
	public:
		Scene();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		void OnUpdate(float dt);
		void OnRender();

		World& GetWorld() { return m_World; }
		const World& GetWorld() const { return m_World; }

		template<typename TSystem, typename... Args>
		TSystem& AddSystem(Args&&... args)
		{
			static_assert(std::is_base_of_v<System, TSystem>,
				"TSystem must derive from System");

			auto system = std::make_unique<TSystem>(std::forward<Args>(args)...);
			TSystem& ref = *system;

			system->OnCreate(*this);
			m_Systems.push_back(std::move(system));

			return ref;
		}

	private:
		World m_World;
		std::vector<std::unique_ptr<System>> m_Systems;
	};
}
#pragma once

#include <entt.hpp>

#include "Core/Timestep.h"

namespace NGN
{
	class eScene
	{
	public:
		eScene();
		~eScene();

		void OnUpdate(Timestep ts);
		void OnRender();

	private:
		entt::registry m_Registry;
	};
}
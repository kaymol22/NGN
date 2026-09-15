#include "System.h"
#include <entt.hpp>

namespace NGN
{
	class TransformSystem : public System
	{
	public:
		~TransformSystem() = default;
		void OnCreate(Scene& scene) override;
		void OnUpdate(Scene& scene, Timestep ts) override;
	};
}
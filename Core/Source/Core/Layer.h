#pragma once 

#include "Events/Event.h"

namespace NGN {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnEvent(Event& event) {}

		virtual void OnUpdate(float timeStep) {}
		virtual void OnRender() {}

		template<std::derived_from<Layer> T, typename ... Args>
		// Takes in template argument, creates new instance of template + its args, calls QueueTransition
		void TransitionTo(Args&& ... args)
		{
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args) ...)));
		}

	private:
		void QueueTransition(std::unique_ptr<Layer> layer);
		std::string m_DebugName;
	};
}
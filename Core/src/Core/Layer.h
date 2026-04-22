#pragma once 

#include "Events/Event.h"
#include "Timestep.h"

namespace NGN {

	class Layer
	{
	public:
		explicit Layer(const std::string name = "Layer")
			: m_DebugName(std::move(name))
		{}

		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnEvent(Event& event) {}

		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}

		template<std::derived_from<Layer> T, typename ... Args>
		// Takes in template argument, creates new instance of template + its args, calls QueueTransition
		void TransitionTo(Args&& ... args)
		{
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args) ...)));
		}

		const std::string& GetName() const { return m_DebugName; }

	private:
		void QueueTransition(std::unique_ptr<Layer> layer);

	private:
		std::string m_DebugName;
	};
}

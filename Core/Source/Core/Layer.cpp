#include "Layer.h"

#include "Application.h"

namespace NGN {

	void Layer::QueueTransition(std::unique_ptr<Layer> toLayer)
	{
		auto* app = &Application::Get();
		// Create Lambda that will run at the end of finished frame in Appliation.cpp run loop
		app->m_PendingTransitions.emplace_back([app, from = this, to = std::move(toLayer)]() mutable {
			// 
			auto& layerStack = app->m_LayerStack;
			for (auto& layer : layerStack)
			{
				if (layer.get() == from)
				{
					layer = std::move(to);
					return;
				}
			}
		});
	}
}
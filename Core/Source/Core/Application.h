#pragma once

#include "Base.h"
#include "Layer.h"
#include "Window.h"
#include "Events/Event.h"

#include <glm/glm.hpp>

#include <set>

namespace NGN {

	struct ApplicationSpecification
	{
		std::string Name = "Application";
		WindowSpecification WindowSpec;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification = ApplicationSpecification());
		~Application();

		void Run();
		void Stop();

		void RaiseEvent(Event& event);

		template<typename TLayer>
			requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer()
		{
			m_LayerStack.push_back(std::make_unique<TLayer>());
		}

		template<typename TLayer>
			requires(std::is_base_of_v<Layer, TLayer>)
		TLayer* GetLayer()
		{
			for (const auto& layer : m_LayerStack)
			{
				if (auto casted = dynamic_cast<TLayer*>(layer.get()))
					return casted;
			}
			return nullptr;
		}

		glm::vec2 GetFramebufferSize() const;

		Window& GetWindow() { return *m_Window; }

		static Application& Get();
		static float GetTime();
	private:
		ApplicationSpecification m_Specification;
		NGN::Scope<Window> m_Window;
		bool m_Running = false;

		// Vector of unique pointers for layers
		// TODO: Evaluate switch to std::stack - though not guarenteed to be contiguous like vector or string*
		std::vector<std::unique_ptr<Layer>> m_LayerStack;

		std::vector<std::move_only_function<void()>> m_PendingTransitions;

		friend class Layer;
	};

}
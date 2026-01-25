#pragma once

#include "Base.h"
#include "Layer.h"
#include "Window.h"
#include "Events/Event.h"
#include "Timestep.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/RendererAPI.h"

#include <glm/glm.hpp>

#include <set>

namespace NGN {

	struct ApplicationSpecification
	{
		std::string Name = "Application";
		WindowSpecification WindowSpec;
		RendererAPIType RendererAPI = RendererAPIType::OpenGL;
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
			auto layer = std::make_unique<TLayer>();
			layer->OnAttach();  // call immediately
			m_LayerStack.push_back(std::move(layer));
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

		Timestep GetTimestep() const { return m_Timestep; }

		static Application& Get();
		static float GetTime();
	private:
		
		// Vector of unique pointers for layers
		// TODO: Evaluate switch to std::stack - though not guarenteed to be contiguous like vector or string*
		std::vector<std::unique_ptr<Layer>> m_LayerStack;
		size_t m_OverlayInsertIndex = 0;

		std::vector<std::move_only_function<void()>> m_PendingTransitions;
	
	private:
		ApplicationSpecification m_Specification;
		
		NGN::Scope<Window> m_Window;
		
		NGN::Ref<ImGuiLayer> m_ImGuiLayer; // TEMP: Testing ImGui

		Timestep m_Timestep;
		
		bool m_Running = false;

		friend class Layer;
	};

}
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

	struct ApplicationCmdLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			NGN_CORE_ASSERT(index < Count, "Index out of Bounds");
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Application";
		WindowSpecification WindowSpec;
		RendererAPIType RendererAPI = RendererAPIType::OpenGL;
		ApplicationCmdLineArgs CommandLineArgs;
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

		Timestep GetTimestep() const { return m_Timestep; }

		glm::vec2 GetFramebufferSize() const;

		Window& GetWindow() { return *m_Window; }

		static Application& Get();

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		// Vector of unique pointers for layers
		// TODO: Evaluate switch to std::stack - though not guarenteed to be contiguous like vector or string*
		std::vector<std::unique_ptr<Layer>> m_LayerStack;
		size_t m_OverlayInsertIndex = 0;

		std::vector<std::move_only_function<void()>> m_PendingTransitions;
	
	private:
		ApplicationSpecification m_Specification;
		
		Scope<Window> m_Window;
		
		Ref<ImGuiLayer> m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;
		Timestep m_Timestep;
		
		bool m_Running = false;
		bool m_Minimized = false;

		friend class Layer;
	};

	// Define in Client + Editor App
	Application* CreateApplication(ApplicationCmdLineArgs args);

}
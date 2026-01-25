#pragma once 

#include "Events/Event.h"
#include "Timestep.h"

namespace NGN {

	enum class LayerFlags : uint32_t
	{
		None		= 0,

		// Render Space - layer can only be one or the other**
		WorldSpace	= 1 << 0,
		ScreenSpace	= 1 << 1,

		// UI
		UsesImGui	= 1 << 2,

		// Development
		Debug		= 1 << 3,
		EditorOnly	= 1 << 4,
	};

	inline LayerFlags operator|(LayerFlags a, LayerFlags b)
	{
		return static_cast<LayerFlags>(
			static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
		);
	}

	class Layer
	{
	public:
		explicit Layer(
			const std::string name = "Layer",
			LayerFlags flags = LayerFlags::None
		)
			: m_DebugName(std::move(name)), m_Flags(flags)
		{}
		
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnEvent(Event& event) {}

		virtual void OnUpdate(Timestep ts) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		template<std::derived_from<Layer> T, typename ... Args>
		// Takes in template argument, creates new instance of template + its args, calls QueueTransition
		void TransitionTo(Args&& ... args)
		{
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args) ...)));
		}

		const std::string& GetName() const { return m_DebugName; }

		LayerFlags GetFlags() const { return m_Flags; }

		bool HasFlag(LayerFlags flag) const
		{
			return (static_cast<uint32_t>(m_Flags) & static_cast<uint32_t>(flag)) != 0;
		}

	private:
		void QueueTransition(std::unique_ptr<Layer> layer);

	private:
		std::string m_DebugName;
		LayerFlags m_Flags;
	};
}
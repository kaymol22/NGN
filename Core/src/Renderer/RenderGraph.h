#pragma once
#include "Core/Types.h"
#include "PassManager.h"

namespace NGN
{
	// Describes Resources - track textures, buffers etc.
	// Define operations
	// Manage dependencies
	// Handle synchronisation 
	// Memeory aliasing & other operations
	class RenderGraph
	{
	public:
		explicit RenderGraph();

		void Build();
		void BeginFrame();
		void Submit(RenderItem&& item);
		void Execute();

		PassManager& GetPassManager() { return m_PassManager; }

	private:
		std::string_view ResolvePassName(const RenderItem& item) const;

		PassManager m_PassManager;
	};

	namespace PassNames
	{
		inline constexpr std::string_view Geometry = "Geometry";
		inline constexpr std::string_view Lighting = "Lighting";
	};
}
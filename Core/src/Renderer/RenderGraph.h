#pragma once
#include "Core/Types.h"
#include "PassManager.h"

namespace NGN
{
	struct alignas(16) FrameUniforms
	{
		glm::mat4 viewProjection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::vec4 camPosition = glm::vec4(0.0f);
	};
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
		Ref<UniformBuffer> m_FrameUBO;
	};

	namespace PassNames
	{
		inline constexpr std::string_view Geometry = "Geometry";
		inline constexpr std::string_view Lighting = "Lighting";
	};
}
#include "RenderGraph.h"
#include "Passes/GeometryPass.h"

namespace NGN
{
	RenderGraph::RenderGraph() = default;

	void RenderGraph::Build()
	{
		const uint32_t width = 1280, height = 720; // TODO: query viewport dimensions from app

		// G-buffer spec creation
		FramebufferSpecification gBufferSpec;
		gBufferSpec.Width = width;
		gBufferSpec.Height = height;
		gBufferSpec.Attachments = {
			FramebufferTextureFormat::RGB16F, // Position
			FramebufferTextureFormat::RGB10A2, // Normals
			FramebufferTextureFormat::SRGB8, // Albedo
			FramebufferTextureFormat::RG16F, // Metallic/Roughness/AO
			FramebufferTextureFormat::Depth
		};

		// Register Passes + set targets
		m_PassManager.AddPass(PassNames::Geometry, GeometryPass::Create());
		m_PassManager.SetTarget(PassNames::Geometry, Framebuffer::Create(gBufferSpec));

		m_PassManager.SetOrder({
			std::string(PassNames::Geometry)
			/*std::string(PassNames::Lighting)*/
		});

		NGN_CORE_INFO("RenderGraph built");
	}

	void RenderGraph::BeginFrame()
	{
		m_PassManager.ClearAll();
	}

	void RenderGraph::Submit(const RenderItem& item)
	{
		std::string_view passName = ResolvePassName(item);
		if (passName.empty()) return;

		if (RenderPass* pass = m_PassManager.GetPass<RenderPass>(passName))
			pass->Submit(item);
	}

	void RenderGraph::Execute()
	{
		m_PassManager.ExecuteAll();
	}

	std::string_view RenderGraph::ResolvePassName(const RenderItem& item) const
	{
		switch (static_cast<ObjectType>(item.objectType))
		{
			case ObjectType::STATIC_MESH:
				return PassNames::Geometry;
			default:
				return {};
		}
	}
}
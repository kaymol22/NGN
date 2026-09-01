#include "RenderGraph.h"
#include "Passes/GeometryPass.h"
#include "Renderer.h"

namespace NGN
{
	RenderGraph::RenderGraph() = default;

	void RenderGraph::Build()
	{
		const uint32_t width = 1280, height = 720; // TODO: query viewport dimensions from app or resolution spec*

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

		// Create and bind at 0 - shared across all passes
		m_FrameUBO = UniformBuffer::Create(sizeof(FrameUniforms), 0);

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

	void RenderGraph::Submit(RenderItem&& item)
	{
		std::string_view passName = ResolvePassName(item);
		if (passName.empty()) return;

		if (RenderPass* pass = m_PassManager.GetPass<RenderPass>(passName))
			pass->Submit(std::move(item));
	}

	void RenderGraph::Execute()
	{
		// Upload uniforms - all passes read from binding 0
		/*if (const SceneCamera* cam = Renderer::GetCurrentCamera())
		{
			FrameUniforms uniforms;
			uniforms.viewProjection = cam->GetViewProjectionMatrix();
			uniforms.view = cam->GetViewMatrix();
			uniforms.projection = cam->GetProjectionMatrix();
			uniforms.camPosition = cam->GetPosition();
			m_FrameUBO->SetData(&uniforms, sizeof(FrameUniforms));
		}*/

		m_PassManager.ExecuteAll();
	}

	std::string_view RenderGraph::ResolvePassName(const RenderItem& item) const
	{
		switch (static_cast<ObjectType>(item.objectType))
		{
			case ObjectType::STATIC_MESH:
			case ObjectType::SKELETAL_MESH:
				return PassNames::Geometry;
			case ObjectType::NO_TYPE:
			case ObjectType::UNDEFINED:
				NGN_CORE_WARN("RenderGraph::ResolvePassName - ObjectType is NO_TYPE or UNDEFINED, cannot resolve pass name");
				return {};
			default:
				return {};
		}
	}
}
#pragma once

#include "Renderer/Framebuffer.h"

namespace NGN
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		// Recreate FB if resized or on first create
		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Spec; }
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override {
			NGN_CORE_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index];
		}
	
	private:
		
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Spec;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
		
		// Color/depth attachments
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}
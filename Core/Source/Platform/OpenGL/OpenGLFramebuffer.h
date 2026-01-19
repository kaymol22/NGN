#pragma once

#include "Renderer/Framebuffer.h"

namespace NGN
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		const FramebufferSpecification& GetSpecification() const { return m_Spec; }
	
	private:
		// Recreate FB if resized or on first create
		void Invalidate();

		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Spec;
		
		// Color/depth attachments
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}
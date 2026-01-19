#include "OpenGLFramebuffer.h"
#include <glad/gl.h>
#include "Core/Log.h"

namespace NGN
{
	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID) 
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		// Color Attachment
		if (m_ColorAttachments.size())
		{

		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
		: m_Spec(specification)
	{
		glCreateFramebuffers(1, & m_RendererID);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{

	}

	void OpenGLFramebuffer::Bind()
	{

	}

	void OpenGLFramebuffer::Unbind()
	{

	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{

	}
}
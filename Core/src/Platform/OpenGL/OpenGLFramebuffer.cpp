#include "OpenGLFramebuffer.h"
#include <glad/gl.h>
#include "Core/Log.h"

namespace NGN
{
	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils
	{
		// MSAAs require different texture target*
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}
		// Batch texture creation - prefer this over individual glGenTextures calls as it reduces driver overhead
		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:		return true;
			}

			return false;
		}
		// Convert NGN framebuffer texture formats to GL enums - for texture creation and glReadPixels calls
		// Data stored in VRAM
		static GLenum NGNFBTextureFormatToGLInternalFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::RGBA8:			return GL_RGBA8;
				case FramebufferTextureFormat::RED_INTEGER:		return GL_R32I;
			}

			NGN_CORE_ASSERT(false, "NGN Framebuffer format conversion to GL internal format failed");
			return 0;
		}
		// Conversion function for glReadPixels - need to read integer data differently from color data
		// Data read back to CPU
		static GLenum NGNFBTextureFormatToGLPixelFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:		return GL_RGBA;
			case FramebufferTextureFormat::RED_INTEGER:	return GL_RED_INTEGER;
			}

			NGN_CORE_ASSERT(false, "NGN Framebuffer format conversion to GL pixel format failed");
			return 0;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
		: m_Spec(specification)
	{
		for (FramebufferTextureSpecification spec : m_Spec.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		// Clear previous
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

		bool multisample = m_Spec.Samples > 1;

		// Set color Attachment based on spec
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			// Configure each color attachment and attach to fb - Max 4 color attachments for now
			for (size_t i = 0; i < m_ColorAttachmentSpecifications.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				FramebufferTextureSpecification& spec = m_ColorAttachmentSpecifications[i];

				GLenum internalFormat = Utils::NGNFBTextureFormatToGLInternalFormat(spec.TextureFormat);
				GLenum pixelFormat = Utils::NGNFBTextureFormatToGLPixelFormat(spec.TextureFormat);

				Utils::AttachColorTexture(m_ColorAttachments[i], m_Spec.Samples, internalFormat, pixelFormat, m_Spec.Width, m_Spec.Height, i);
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Spec.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Spec.Width, m_Spec.Height);
				break;
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			// Individual draw buffers for each color attachment - need for Multiple Render Targets
			NGN_CORE_ASSERT(m_ColorAttachments.size() <= 4, "Currently only 4 color attachments supported");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		NGN_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Spec.Width, m_Spec.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			NGN_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Spec.Width = width;
		m_Spec.Height = height;

		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		NGN_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Color attachment index out of bounds");
		
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

		FramebufferTextureSpecification& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		GLenum pixelFormat = Utils::NGNFBTextureFormatToGLPixelFormat(spec.TextureFormat);
		
		int pixelData;
		glReadPixels(x, y, 1, 1, pixelFormat, GL_INT, &pixelData);

		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		NGN_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Color attachment index out of bounds");

		FramebufferTextureSpecification& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		GLenum pixelFormat = Utils::NGNFBTextureFormatToGLPixelFormat(spec.TextureFormat);

		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, pixelFormat, GL_INT, &value);
	}
}
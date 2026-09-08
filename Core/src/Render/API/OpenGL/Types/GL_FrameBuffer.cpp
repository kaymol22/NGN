#include "GL_Framebuffer.h"
#include "Render/API/OpenGL/GL_Utils.h"

namespace OpenGL
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const std::string& name, int width, int height, uint32_t sampleCount)
	{
		Create(name, width, height, sampleCount);
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const std::string& name, const glm::ivec2 resolution, uint32_t sampleCount)
	{
		Create(name, resolution, sampleCount);
	}

	void OpenGLFrameBuffer::SetName(const std::string& name)
	{
		m_Name = name;
	}

	void OpenGLFrameBuffer::Create(int width, int height, uint32_t sampleCount)
	{
		const std::string name = m_Name;
		CleanUp();

		glCreateFramebuffers(1, &m_Handle);
		m_Name = name;
		m_Width = width;
		m_Height = height;
		m_SampleCount = sampleCount;
	}

	void OpenGLFrameBuffer::Create(const glm::ivec2 resolution, uint32_t sampleCount)
	{
		Create(resolution.x, resolution.y, sampleCount);
	}

	void OpenGLFrameBuffer::Create(const std::string& name, int width, int height, uint32_t sampleCount)
	{
		SetName(name);
		Create(width, height, sampleCount);
	}

	void OpenGLFrameBuffer::Create(const std::string& name, const glm::ivec2 resolution, uint32_t sampleCount)
	{
		Create(name, resolution.x, resolution.y, sampleCount);
	}

	void OpenGLFrameBuffer::CleanUp()
	{
		for (ColorAttachment& attachment : m_ColorAttachments) {
			if (attachment.handle != 0) {
				glDeleteTextures(1, &attachment.handle);
			}
		}
		m_ColorAttachments.clear();

		if (m_DepthAttachment.handle != 0) {
			glDeleteTextures(1, &m_DepthAttachment.handle);
		}

		if (m_Handle != 0) glDeleteFramebuffers(1, &m_Handle);

		m_Width = 0;
		m_Height = 0;
		m_SampleCount = 0;
		m_Handle = 0;
		m_DepthAttachment.handle = 0;

		m_CachedAttachmentHandles.clear();
		m_CachedAttachmentSlots.clear();
	}

	void OpenGLFrameBuffer::CreateAttachment(const std::string& name, GLenum internalFormat, GLenum minFilter,
		GLenum magFilter, GLenum wrapFilter, bool allocateMips)
	{
		GLenum target = IsMultiSampled() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

		ColorAttachment& colorAttachment = m_ColorAttachments.emplace_back();
		colorAttachment.name = name;
		colorAttachment.internalFormat = internalFormat;
		colorAttachment.format = Utils::GLInternalFormatToGLFormat(internalFormat);
		colorAttachment.type = Utils::GLInternalFormatToGLType(internalFormat);
		colorAttachment.target = target;
		colorAttachment.minFilter = minFilter;
		colorAttachment.magFilter = magFilter;
		colorAttachment.wrapFilter = wrapFilter;
		colorAttachment.allocateMips = allocateMips;

		glCreateTextures(target, 1, &colorAttachment.handle);

		if (IsMultiSampled()) {
			glTextureStorage2DMultisample(colorAttachment.handle, m_SampleCount, internalFormat, m_Width, m_Height, GL_TRUE);
		}
		else {
			int levels = 1;
			if (allocateMips) {
				int maxDim = std::max(m_Width, m_Height);
				levels = 1 + (int)floor(log2(maxDim));
			}

			glTextureStorage2D(colorAttachment.handle, levels, internalFormat, m_Width, m_Height);
			glTextureParameteri(colorAttachment.handle, GL_TEXTURE_MIN_FILTER, allocateMips ? GL_LINEAR_MIPMAP_LINEAR : minFilter);
			glTextureParameteri(colorAttachment.handle, GL_TEXTURE_MAG_FILTER, magFilter);
			glTextureParameteri(colorAttachment.handle, GL_TEXTURE_WRAP_S, wrapFilter);
			glTextureParameteri(colorAttachment.handle, GL_TEXTURE_WRAP_T, wrapFilter);
		}

		GLenum attachment = GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(m_ColorAttachments.size() - 1);
		glNamedFramebufferTexture(m_Handle, attachment, colorAttachment.handle, 0);

		std::string debugLabel = "Texture (FBO: " + m_Name + "Tex: " + name + ")";
		glObjectLabel(GL_TEXTURE, colorAttachment.handle, static_cast<GLsizei>(debugLabel.length()), debugLabel.c_str());
	}

	void OpenGLFrameBuffer::CreateDepthAttachment(GLenum internalFormat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter, glm::vec4 borderColor)
	{
		m_DepthAttachment.internalFormat = internalFormat;
		m_DepthAttachment.minFilter = minFilter;
		m_DepthAttachment.magFilter = magFilter;
		m_DepthAttachment.wrapFilter = wrapFilter;
		m_DepthAttachment.borderColor = borderColor;

		GLenum target = IsMultiSampled() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		glCreateTextures(target, 1, &m_DepthAttachment.handle);

		if (IsMultiSampled()) {
			glTextureStorage2DMultisample(m_DepthAttachment.handle, m_SampleCount, internalFormat, m_Width, m_Height, GL_TRUE);
		}
		else {
			glTextureStorage2D(m_DepthAttachment.handle, 1, internalFormat, m_Width, m_Height);
			glTextureParameteri(m_DepthAttachment.handle, GL_TEXTURE_MIN_FILTER, minFilter);
			glTextureParameteri(m_DepthAttachment.handle, GL_TEXTURE_MAG_FILTER, magFilter);
			glTextureParameteri(m_DepthAttachment.handle, GL_TEXTURE_WRAP_S, wrapFilter);
			glTextureParameteri(m_DepthAttachment.handle, GL_TEXTURE_WRAP_T, wrapFilter);
			glTextureParameterfv(m_DepthAttachment.handle, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
		}

		GLenum attachmentPoint = (internalFormat == GL_DEPTH24_STENCIL8 || internalFormat == GL_DEPTH32F_STENCIL8)
			? GL_DEPTH_STENCIL_ATTACHMENT
			: GL_DEPTH_ATTACHMENT;

		glNamedFramebufferTexture(m_Handle, attachmentPoint, m_DepthAttachment.handle, 0);

		std::string debugLabel = "Texture: (FBO: " + m_Name + " Tex: Depth)";
		glObjectLabel(GL_TEXTURE, m_DepthAttachment.handle, static_cast<GLsizei>(debugLabel.length()), debugLabel.c_str());
	}

	void OpenGLFrameBuffer::BindDepthAttachmentFrom(const OpenGLFrameBuffer& srcFrameBuffer)
	{
		GLenum attach = (srcFrameBuffer.m_DepthAttachment.internalFormat == GL_DEPTH24_STENCIL8 ||
			srcFrameBuffer.m_DepthAttachment.internalFormat == GL_DEPTH32F_STENCIL8)
			? GL_DEPTH_STENCIL_ATTACHMENT
			: GL_DEPTH_ATTACHMENT;

		glNamedFramebufferTexture(m_Handle, attach, srcFrameBuffer.m_DepthAttachment.handle, 0);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
	}

	void OpenGLFrameBuffer::SetViewport()
	{
		glViewport(0, 0, m_Width, m_Height);
	}

	void OpenGLFrameBuffer::DrawBuffer(GLenum buffer)
	{
		m_DrawBuffers = { buffer };
		glDrawBuffer(buffer);
	}

	void OpenGLFrameBuffer::DrawBuffer(const std::string& attachmentName)
	{
		DrawBuffer(GetColorAttachmentSlotByName(attachmentName));
	}

	void OpenGLFrameBuffer::DrawBuffers(const std::vector<std::string>& attachmentNames)
	{
		m_DrawBuffers.clear();
		for (const std::string& attachmentName : attachmentNames) {
			m_DrawBuffers.push_back(GetColorAttachmentSlotByName(attachmentName));
		}
		glDrawBuffers(static_cast<GLsizei>(m_DrawBuffers.size()), m_DrawBuffers.data());
	}

	void OpenGLFrameBuffer::ClearAttachment(const std::string& attachmentName, float r, float g, float b, float a)
	{
		for (int i = 0; i < (int)m_ColorAttachments.size(); i++) {
			if (attachmentName == m_ColorAttachments[i].name) {
				float clearColor[4] = { r, g, b, a };
				GLenum drawBuffer = GL_COLOR_ATTACHMENT0 + i;
				glNamedFramebufferDrawBuffers(m_Handle, 1, &drawBuffer);
				glClearNamedFramebufferfv(m_Handle, GL_COLOR, 0, clearColor);
				return;
			}
		}
	}

	void OpenGLFrameBuffer::ClearAttachmentR(const std::string& attachmentName, GLfloat r)
	{
		ClearAttachment(attachmentName, r, 0.0f, 0.0f, 0.0f);
	}

	void OpenGLFrameBuffer::ClearTexImage(const std::string& attachmentName, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		int index = -1;
		for (int i = 0; i < m_ColorAttachments.size(); i++) {
			if (attachmentName == m_ColorAttachments[i].name) {
				index = i;
				break;
			}
		}
		if (index >= 0) {
			GLuint tex = m_ColorAttachments[index].handle;
			float clearColor[4] = { r, g, b, a };
			glClearTexImage(tex, 0, GL_RGBA, GL_FLOAT, clearColor);
		}
		else {
			std::cout << "OpenGLFrameBuffer::ClearTexImage() failed: '" << attachmentName << "' not found\n";
		}
	}

	void OpenGLFrameBuffer::ClearAttachmentI(const std::string& attachmentName, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		for (int i = 0; i < m_ColorAttachments.size(); i++) {
			if (attachmentName == m_ColorAttachments[i].name) {
				GLuint texture = m_ColorAttachments[i].handle;
				GLenum format = m_ColorAttachments[i].format;
				GLenum type = m_ColorAttachments[i].type;
				GLint clearColor[4] = { r, g, b, a };
				glClearTexSubImage(texture, 0, 0, 0, 0, GetWidth(), GetHeight(), 1, format, type, clearColor);
				return;
			}
		}
	}

	void OpenGLFrameBuffer::ClearAttachmentSubRegion(const std::string& attachmentName, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height,
		GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		for (int i = 0; i < m_ColorAttachments.size(); i++) {
			if (attachmentName == m_ColorAttachments[i].name) {
				GLuint texture = m_ColorAttachments[i].handle;
				GLenum format = m_ColorAttachments[i].format;
				GLenum type = m_ColorAttachments[i].type;
				GLfloat clearColor[4] = { r, g, b, a };
				glClearTexSubImage(texture, 0, xOffset, yOffset, 0, width, height, 1, format, type, clearColor);
				return;
			}
		}
	}

	void OpenGLFrameBuffer::ClearDepthAttachment()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLFrameBuffer::ClearDepthAttachment(float value)
	{
		if (m_DepthAttachment.handle == 0) return;

		glClearNamedFramebufferfv(m_Handle, GL_DEPTH, 0, &value);
	}

	void OpenGLFrameBuffer::ClearStencilBits(GLint value)
	{
		if (m_DepthAttachment.handle == 0) return;

		glStencilMask(0xFF);
		glClearNamedFramebufferiv(m_Handle, GL_STENCIL, 0, &value);
	}

	void OpenGLFrameBuffer::Resize(int width, int height)
	{
		if (width <= 0 || height <= 0) return;

		std::string name = m_Name;
		GLuint sampleCount = m_SampleCount;
		std::vector<ColorAttachment> colorAttachments = m_ColorAttachments;
		DepthAttachment depthAttachment = m_DepthAttachment;

		CleanUp();
		SetName(name);
		Create(width, height, sampleCount);

		for (const ColorAttachment& colorAttachment : colorAttachments) {
			CreateAttachment(colorAttachment.name, colorAttachment.internalFormat, colorAttachment.minFilter, colorAttachment.magFilter, colorAttachment.wrapFilter, colorAttachment.allocateMips);
		}

		if (depthAttachment.internalFormat != GL_NONE) {
			CreateDepthAttachment(depthAttachment.internalFormat, depthAttachment.minFilter, depthAttachment.magFilter, depthAttachment.wrapFilter, depthAttachment.borderColor);
		}
	}

	void OpenGLFrameBuffer::SetColorAttachmentMipLevel(const std::string& attachmentName, int mipLevel)
	{
		for (int i = 0; i < m_ColorAttachments.size(); i++) {
			if (attachmentName == m_ColorAttachments[i].name) {
				GLenum attachment = GL_COLOR_ATTACHMENT0 + i;
				glNamedFramebufferTexture(m_Handle, attachment, m_ColorAttachments[i].handle, mipLevel);
				return;
			}
		}
	}

	GLint OpenGLFrameBuffer::GetColorAttachmentHandleByName(const std::string& name)
	{
		// Check cache first
		auto it = m_CachedAttachmentHandles.find(name);
		if (it != m_CachedAttachmentHandles.end()) {
			return it->second;
		}
		// Loop through attachments and add to cache for faster retrieval
		for (int i = 0; i < m_ColorAttachments.size(); i++) {
			if (name == m_ColorAttachments[i].name) {
				m_CachedAttachmentHandles[name] = m_ColorAttachments[i].handle;
				return m_ColorAttachments[i].handle;
			}
		}

		NGN_CORE_ERROR("OpenGLFrameBuffer::GetColorAttachmentHandleByName() with name '", name, "' failed. Name doesn't exist in FrameBuffer '", this->m_Name, "'");
		return GL_NONE;
	}

	GLenum OpenGLFrameBuffer::GetColorAttachmentSlotByName(const std::string& name)
	{
		auto it = m_CachedAttachmentSlots.find(name);
		if (it != m_CachedAttachmentSlots.end()) {
			return it->second;
		}

		for (int i = 0; i < m_ColorAttachments.size(); i++) {
			if (name == m_ColorAttachments[i].name) {
				GLenum slot = GL_COLOR_ATTACHMENT0 + i;
				m_CachedAttachmentSlots[name] = slot;
				return slot;
			}
		}

		NGN_CORE_ERROR("OpenGLFrameBuffer::GetColorAttachmentSlotByName() with name '", name, "' failed. Name doesn't exist in FrameBuffer '", this->m_Name, "'");
		return GL_INVALID_VALUE;
	}

	void OpenGLFrameBuffer::BlitToDefaultFrameBuffer(const std::string& srcName, GLint dstX0, GLint dstY0,
		GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, GetHandle());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glReadBuffer(GetColorAttachmentSlotByName(srcName));
		glDrawBuffer(GL_BACK);
		glBlitFramebuffer(0, 0, GetWidth(), GetHeight(), dstX0, dstY0, dstX1, dstY1, mask, filter);
	}

	size_t OpenGLFrameBuffer::GetGPUAllocatedByteCount()
	{
		size_t byteCount = 0;

		for (const ColorAttachment& colorAttachment : m_ColorAttachments) {
			if (colorAttachment.handle == 0) {
				continue;
			}

			uint32_t mipMapLevelCount = 1;
			if (colorAttachment.allocateMips && IsMultiSampled()) {
				const GLuint maxDim = std::max(m_Width, m_Height);
				mipMapLevelCount = maxDim > 0 ? 1 + static_cast<uint32_t>(std::floor(std::log2(maxDim))) : 0;
			}

			byteCount += Utils::CalculateTexture2DByteCount(m_Width, m_Height, colorAttachment.internalFormat, mipMapLevelCount, m_SampleCount);
		}

		if (m_DepthAttachment.handle != 0) {
			byteCount += Utils::CalculateTexture2DByteCount(m_Width, m_Height, m_DepthAttachment.internalFormat, 1, m_SampleCount);
		}

		return byteCount;
	}
}
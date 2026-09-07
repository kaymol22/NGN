#pragma once
#include <glm/glm.hpp>
#include <glad/gl.h>

#include "GL_Attachments.h"

namespace OpenGL
{
	struct OpenGLFrameBuffer {
		OpenGLFrameBuffer() = default;
		OpenGLFrameBuffer(const std::string& name, int width, int height, uint32_t sampleCount = 1);
		OpenGLFrameBuffer(const std::string& name, const glm::ivec2 resolution, uint32_t sampleCount = 1);

		void SetName(const std::string& name);
		void Create(int width, int height, uint32_t sampleCount = 1);
		void Create(const glm::ivec2 resolution, uint32_t sampleCount = 1);
		void Create(const std::string& name, int width, int height, uint32_t sampleCount = 1);
		void Create(const std::string& name, const glm::ivec2 resolution, uint32_t sampleCount = 1);
		void CleanUp();
		void CreateAttachment(const std::string& name, GLenum internalFormat, GLenum minFilter = GL_LINEAR,
			GLenum magFilter = GL_LINEAR, GLenum wrapMode = GL_CLAMP_TO_EDGE, bool allocateMips = false);
		void CreateAttachment(GLenum internalFormat, GLenum minFilter = GL_LINEAR,
			GLenum magFilter = GL_LINEAR, GLenum wrapMode = GL_CLAMP_TO_EDGE, glm::vec4 borderColor = glm::vec4(1.0f));
		void BindDepthAttachmentFrom(const OpenGLFrameBuffer& srcFrameBuffer);
		void Bind();
		void SetViewport();
		void DrawBuffer(GLenum buffer);
		void DrawBuffer(const std::string& attachmentName);
		void DrawBuffers(const std::vector<std::string>& attachmentNames);
		void ClearAttachment(const std::string& attachmentName, float r, float g, float b, float a);
		void ClearAttachmentR(const std::string& attachmentName, GLfloat r);
		void ClearTexImage(const std::string& attachmentName, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		void ClearAttachmentI(const std::string& attachmentName, GLfloat r, GLfloat g = 0, GLfloat b = 0, GLfloat a = 0);
		void ClearAttachmentSubRegion(const std::string& attachmentName, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height,
			GLfloat r, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 0.0f);
		void ClearDepthAttachment();
		void ClearDepthAttachment(float value);
		void ClearStencilBits(GLint value);
		void Resize(int width, int height);
		void SetColorAttachmentMipLevel(const std::string& attachmentName, int mipLevel);

		GLint GetColorAttachmentHandleByName(const std::string& name);
		GLenum GetColorAttachmentSlotByName(const std::string& name);
		void BlitToDefaultFrameBuffer(const std::string& srcName, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

		GLuint GetHandle() const { return m_Handle; }
		GLuint GetWidth() const { return m_Width; }
		GLuint GetHeight() const { return m_Height; }
		GLuint GetDepthAttachmentHandle() const { return m_DepthAttachment.handle; }
		bool IsMultiSampled() { return m_SampleCount > 1; }
		size_t GetGPUAllocatedByteCount();

	private:
		std::string name = "Undefined";
		GLuint m_Handle = 0;
		GLuint m_Width = 0;
		GLuint m_Height = 0;
		GLuint m_SampleCount = 1;
		std::vector<ColorAttachment> m_ColorAttachments;
		DepthAttachment m_DepthAttachment;
		std::unordered_map<std::string, GLuint> m_CachedAttachmentHandles;
		std::unordered_map<std::string, GLuint> m_CachedAttachmentSlots;
		std::vector<GLenum> m_DrawBuffers;
	};
}
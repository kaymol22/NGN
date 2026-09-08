#include "GL_ssbo.h"

namespace OpenGL
{
	OpenGLSSBO::OpenGLSSBO(size_t size, GLbitfield flags)
		: m_Size(size), m_Flags(flags)
	{
		Create(size, flags);
	}

	void OpenGLSSBO::Create(size_t size, GLbitfield flags)
	{
		CleanUp();
		m_Flags = flags;
		Reserve(size);
	}

	void OpenGLSSBO::Reserve(size_t size)
	{
		if (m_Handle != 0 && m_Size >= size) {
			return; // Already allocated enough space
		}
		CleanUp();
		glCreateBuffers(1, &m_Handle);
		glNamedBufferStorage(m_Handle, (GLsizeiptr)size, nullptr, m_Flags);
		m_Size = size;
	}

	void OpenGLSSBO::Update(size_t size, const void* data)
	{
		if (size == 0 || !data) return;

		Reserve(size);
		glNamedBufferSubData(m_Handle, 0, (GLsizeiptr)size, data);
	}

	void OpenGLSSBO::UpdateRange(size_t offset, size_t size, const void* data)
	{
		if (size == 0 || !data) return;
		
		if (offset + size > m_Size) {
			NGN_CORE_ERROR("OpenGLSSBO::UpdateRange - Range exceeds buffer size");
			return;
		}
		Reserve(offset + size);
		glNamedBufferSubData(m_Handle, (GLintptr)offset, (GLsizeiptr)size, data);
	}

	void OpenGLSSBO::UploadStatic(size_t size, const void* data)
	{
		if (size == 0 || !data) return;

		CleanUp();
		glCreateBuffers(1, &m_Handle);
		glNamedBufferStorage(m_Handle, (GLsizeiptr)size, data, 0);

		m_Size = size;
		m_Flags = 0; // Static storage - no flags
	}

	void OpenGLSSBO::Bind(uint32_t index) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_Handle);
	}

	void OpenGLSSBO::CleanUp()
	{
		if (m_Handle != 0) {
			glDeleteBuffers(1, &m_Handle);
			m_Handle = 0;
			m_Size = 0;
			m_Flags = 0;
		}
	}
	
	void OpenGLSSBO::CopyFrom(const void* hostPtr, size_t sizeInBytes) const
	{
		if (!hostPtr || sizeInBytes == 0 || m_Handle == 0) return;

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Handle);
		void* ptr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeInBytes, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		if (!ptr) {
			NGN_CORE_ERROR("OpenGLSSBO::CopyFrom - Failed to map buffer");
			return;
		}

		std::memcpy(ptr, hostPtr, sizeInBytes);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void OpenGLSSBO::Clear() const
	{
		if (m_Handle == 0) return;
		uint32_t zero = 0;
		glClearNamedBufferData(m_Handle, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &zero);
	}

	void OpenGLSSBO::ClearRange(size_t offset, size_t size) const
	{
		if (m_Handle == 0 || offset >= m_Size || size == 0) return;

		size_t clearSize = (offset + size > m_Size) ? (m_Size - offset) : size;
		uint32_t zero = 0;
		glClearNamedBufferSubData(m_Handle, GL_R32UI, (GLintptr)offset, (GLsizeiptr)clearSize, GL_RED_INTEGER, GL_UNSIGNED_INT, &zero);
	}
}
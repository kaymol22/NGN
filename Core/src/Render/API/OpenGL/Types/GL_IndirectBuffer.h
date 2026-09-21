#pragma once
#include <glad/gl.h>
#include <cstdint>

namespace OpenGL
{
	struct IndirectBuffer {
	public:
		uint32_t GetHandle() {
			return m_Handle;
		}

		void PreAllocate(size_t size) {
			CleanUp();
			glCreateBuffers(1, &m_Handle);
			glNamedBufferStorage(m_Handle, (GLsizeiptr)size, nullptr, GL_DYNAMIC_STORAGE_BIT);
			m_BufferSize = size;
		}

		void Update(size_t size, const void* data) {
			if (size == 0) {
				return;
			}

			if (m_Handle == 0 || m_BufferSize < size) {
				CleanUp();
				glCreateBuffers(1, &m_Handle);
				glNamedBufferStorage(m_Handle, (GLsizeiptr)size, nullptr, GL_DYNAMIC_STORAGE_BIT);
				m_BufferSize = size;
			}
			// Upload new data
			glNamedBufferSubData(m_Handle, 0, (GLsizeiptr)size, data);
			glMemoryBarrier(GL_COMMAND_BARRIER_BIT);
		}

		void CleanUp() {
			if (m_Handle != 0) {
				glDeleteBuffers(1, &m_Handle);
				m_Handle = 0;
				m_BufferSize = 0;
			}
		}

		void Bind() {
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_Handle);
		}

		void CopyFromSSBO(uint32_t ssboHandle, size_t size) {
			glBindBuffer(GL_COPY_READ_BUFFER, ssboHandle);
			glBindBuffer(GL_COPY_WRITE_BUFFER, m_Handle);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, (GLsizeiptr)size);
		}

	private:
		uint32_t m_Handle = 0;
		size_t m_BufferSize = 0;
	};
}
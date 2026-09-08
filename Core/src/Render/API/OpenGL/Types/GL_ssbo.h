#pragma once
#include <glad/gl.h>
#include <cstddef>
#include <cstdint>

namespace OpenGL
{
	struct OpenGLSSBO
	{
		OpenGLSSBO() = default;
		OpenGLSSBO(size_t size, GLbitfield flags);

		void Create(size_t size, GLbitfield flags);
		void Reserve(size_t size);
		void Update(size_t size, const void* data);
		void UpdateRange(size_t offset, size_t size, const void* data);
		void UploadStatic(size_t size, const void* data);
		void Bind(uint32_t index) const;
		void CleanUp();
		void CopyFrom(const void* hostPtr, size_t sizeInBytes) const;
		void Clear() const;
		void ClearRange(size_t offset, size_t size) const;

	private:
		uint32_t m_Handle = 0;
		size_t m_Size = 0;
		GLbitfield m_Flags = 0;
	};
}
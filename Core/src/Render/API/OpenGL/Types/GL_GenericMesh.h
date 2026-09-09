#pragma once

#include "Render/VertexAttributes.h"

namespace OpenGL
{
	struct OpenGLGenericMesh {
		void UpdateVertexData(const void* vertices, size_t vertexCount, const VertexLayoutDescription& layout);
		void UpdateIndexData(const std::vector<uint32_t>& indices);
		void CleanUp();

		size_t GetVertexCount() { return m_VertexCount; }
		size_t GetIndexCount() { return m_IndexCount; }
		uint32_t GetVAO() const { return m_VAO; }
		uint32_t GetVBO() const { return m_VBO; }
		uint32_t GetEBO() const { return m_EBO; }

	private:
		void Init(const VertexLayoutDescription& layout);
		void ResizeVertexBuffer(size_t newCapacity);
		void ResizeIndexBuffer(size_t newCapacity);

		uint32_t m_VAO = 0;
		uint32_t m_VBO = 0;
		uint32_t m_EBO = 0;
		size_t m_VertexStride = 0;
		size_t m_VertexCount = 0;
		size_t m_IndexCount = 0;
		size_t m_VertexCapacity = 0;
		size_t m_IndexCapacity = 0;
	};
}
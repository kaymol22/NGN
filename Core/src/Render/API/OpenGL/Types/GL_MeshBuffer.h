#pragma once
#include "Render/VertexAttributes.h"

namespace OpenGL
{
	struct OpenGLMeshBuffer {
		void Init(const VertexLayoutDescription& layout);
		void Reset();
		void InsertVertices(const std::vector<Vertex>& vertices, uint32_t insertOffset);
		void InsertIndices(const std::vector<uint32_t>& indices, uint32_t insertOffset);
		void PreAllocate(size_t vertexCapacity, size_t indexCapacity);

		uint32_t GetVAO() const { return m_VAO; }
		uint32_t GetVBO() const { return m_VBO; }
		uint32_t GetEBO() const { return m_EBO; }
		size_t GetVertexStride() const { return m_VertexStride; }
	private:
		void GrowVertexBuffer(size_t newCapacity);
		void GrowIndexBuffer(size_t newCapacity);

	private:
		uint32_t m_VAO = 0;
		uint32_t m_VBO = 0;
		uint32_t m_EBO = 0;
		size_t m_VertexStride = 0;
		size_t m_VertexCapacity = 0;
		size_t m_IndexCapacity = 0;
	};
}
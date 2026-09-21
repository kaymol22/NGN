#include "GL_MeshBuffer.h"
#include <glad/gl.h>

namespace OpenGL
{
	void OpenGLMeshBuffer::Init(const VertexLayoutDescription& layout)
	{
		if (m_VAO != 0) {
			Reset();
		}

		m_VertexStride = layout.stride;
		glCreateVertexArrays(1, &m_VAO);

		for (const VertexAttribute& attribute : layout.attributes)
		{
			glEnableVertexArrayAttrib(m_VAO, attribute.location);

			GLenum type = GL_FLOAT;
			switch (attribute.type) {
				case VertexAttributeType::Float:			type = GL_FLOAT;			break;
				case VertexAttributeType::Int:				type = GL_INT;				break;
				case VertexAttributeType::UnsignedInt:		type = GL_UNSIGNED_INT;		break;
			}
			if (attribute.type == VertexAttributeType::Float) {
				glVertexArrayAttribFormat(m_VAO, attribute.location, attribute.componentCount, type,
					attribute.normalized ? GL_TRUE : GL_FALSE, static_cast<GLuint>(attribute.offset));
			}
			else {
				glVertexArrayAttribIFormat(m_VAO, attribute.location, attribute.componentCount, type, static_cast<GLuint>(attribute.offset));
			}

			glVertexArrayAttribBinding(m_VAO, attribute.location, 0);
		}
	}

	void OpenGLMeshBuffer::Reset()
	{
		if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
		if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
		if (m_EBO != 0) glDeleteBuffers(1, &m_EBO);

		m_VAO = 0;
		m_VBO = 0;
		m_EBO = 0;
		m_VertexStride = 0;
	}

	void OpenGLMeshBuffer::InsertVertices(const std::vector<Vertex>& vertices, uint32_t insertOffset) {
		if (vertices.empty()) return;

		size_t reqCapacity = insertOffset + vertices.size();
		if (reqCapacity > m_VertexCapacity) {
			GrowVertexBuffer(reqCapacity);
		}

		size_t byteOffset = insertOffset * m_VertexStride;
		size_t byteSize = vertices.size() * m_VertexStride;

		glNamedBufferSubData(m_VBO, byteOffset, byteSize, vertices.data());
	}

	void OpenGLMeshBuffer::InsertIndices(const std::vector<uint32_t>& indices, uint32_t insertOffset) {
		if (indices.empty()) return;

		size_t reqCapacity = insertOffset + indices.size();
		if (reqCapacity > m_IndexCapacity) {
			GrowIndexBuffer(reqCapacity);
		}

		size_t byteOffset = insertOffset * sizeof(uint32_t);
		size_t byteSize = indices.size() * sizeof(uint32_t);

		glNamedBufferSubData(m_EBO, byteOffset, byteSize, indices.data());
	}

	void OpenGLMeshBuffer::PreAllocate(size_t vertexCapacity, size_t indexCapacity) {
		if (m_VertexCapacity > 0 || m_IndexCapacity > 0) {
			NGN_CORE_WARN("OpenGLMeshBuffer::PreAllocate() called on already allocated buffer - any existing data discarded");
		}

		if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
		if (m_EBO != 0) glDeleteBuffers(1, &m_EBO);

		glCreateBuffers(1, &m_VBO);
		glCreateBuffers(1, &m_EBO);

		glNamedBufferStorage(m_VBO, vertexCapacity * m_VertexStride, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferStorage(m_EBO, indexCapacity * sizeof(uint32_t), nullptr, GL_DYNAMIC_STORAGE_BIT);

		glVertexArrayElementBuffer(m_VAO, m_EBO);
		glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, static_cast<GLsizei>(m_VertexStride));

		m_VertexCapacity = vertexCapacity;
		m_IndexCapacity = indexCapacity;
	}

	void OpenGLMeshBuffer::GrowVertexBuffer(size_t newCapacity) {
		GLuint newVBO = 0;
		glCreateBuffers(1, &newVBO);
		glNamedBufferStorage(newVBO, newCapacity * m_VertexStride, nullptr, GL_DYNAMIC_STORAGE_BIT);

		if (m_VBO != 0 && m_VertexCapacity > 0) {
			glCopyNamedBufferSubData(m_VBO, newVBO, 0, 0, m_VertexCapacity * m_VertexStride);
			glDeleteBuffers(1, &m_VBO);
		}

		m_VBO = newVBO;
		m_VertexCapacity = newCapacity;
		glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, static_cast<GLsizei>(m_VertexStride));
	}

	void OpenGLMeshBuffer::GrowIndexBuffer(size_t newCapacity) {
		GLuint newEBO = 0;
		glCreateBuffers(1, &newEBO);
		glNamedBufferStorage(newEBO, newCapacity * sizeof(uint32_t), nullptr, GL_DYNAMIC_STORAGE_BIT);

		if (m_EBO != 0 && m_IndexCapacity > 0) {
			glCopyNamedBufferSubData(m_EBO, newEBO, 0, 0, m_IndexCapacity * sizeof(uint32_t));
			glDeleteBuffers(1, &m_EBO);
		}

		m_EBO = newEBO;
		m_IndexCapacity = newCapacity;
		glVertexArrayElementBuffer(m_VAO, m_EBO);
	}
}
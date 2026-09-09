#include <glad/gl.h>
#include "GL_GenericMesh.h"

namespace OpenGL
{
	void OpenGLGenericMesh::UpdateVertexData(const void* vertices, size_t vertexCount, const VertexLayoutDescription& layout) {
		if (m_VAO == 0) {
			Init(layout);
		}

		m_VertexCount = vertexCount;
		if (vertexCount == 0) {
			return;
		}

		if (vertexCount > m_VertexCapacity) {
			ResizeVertexBuffer(vertexCount);
		}

		glNamedBufferSubData(m_VBO, 0, vertexCount * m_VertexStride, vertices);
	}

	void OpenGLGenericMesh::UpdateIndexData(const std::vector<uint32_t>& indices) {
		m_IndexCount = indices.size();

		if (indices.empty()) {
			return;
		}

		if (indices.size() > m_IndexCapacity) {
			ResizeIndexBuffer(indices.size());
		}

		glNamedBufferSubData(m_EBO, 0, indices.size() * sizeof(uint32_t), indices.data());
	}

	void OpenGLGenericMesh::CleanUp() {
		if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
		if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
		if (m_EBO != 0) glDeleteBuffers(1, &m_EBO);

		m_VAO = 0;
		m_VBO = 0;
		m_EBO = 0;
		m_VertexStride = 0;
		m_VertexCount = 0;
		m_IndexCount = 0;
		m_VertexCapacity = 0;
		m_IndexCapacity = 0;
	}

	void OpenGLGenericMesh::Init(const VertexLayoutDescription& layout) {
		m_VertexStride = layout.stride;
		glCreateVertexArrays(1, &m_VAO);

		for (const VertexAttribute& attribute : layout.attributes) {
			GLenum type = GL_FLOAT;
			switch (attribute.type) {
				case VertexAttributeType::Float:
					type = GL_FLOAT;
					break;
				case VertexAttributeType::Int:
					type = GL_INT;
					break;
				case VertexAttributeType::UnsignedInt:
					type = GL_UNSIGNED_INT;
					break;
			}

			if (attribute.type == VertexAttributeType::Float) {
				glVertexArrayAttribFormat(m_VAO, attribute.location, attribute.componentCount, type, attribute.normalized ? GL_TRUE : GL_FALSE, static_cast<GLuint>(attribute.offset));
			}
			else {
				glVertexArrayAttribIFormat(m_VAO, attribute.location, attribute.componentCount, type, static_cast<GLuint>(attribute.offset));
			}

			glVertexArrayAttribBinding(m_VAO, attribute.location, 0);
		}

		if (m_EBO != 0) {
			glVertexArrayElementBuffer(m_VAO, m_EBO);
		}
	}

	void OpenGLGenericMesh::ResizeVertexBuffer(size_t newCapacity) {
		uint32_t newVBO = 0;
		glCreateBuffers(1, &newVBO);
		glNamedBufferStorage(newVBO, newCapacity * m_VertexStride, nullptr, GL_DYNAMIC_STORAGE_BIT);

		if (m_VBO != 0) {
			glDeleteBuffers(1, &m_VBO);
		}

		m_VBO = newVBO;
		m_VertexCapacity = newCapacity;

		glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, static_cast<GLsizei>(m_VertexStride));
	}

	void OpenGLGenericMesh::ResizeIndexBuffer(size_t newCapacity) {
		uint32_t newEBO = 0;
		glCreateBuffers(1, &newEBO);
		glNamedBufferStorage(newEBO, newCapacity * sizeof(uint32_t), nullptr, GL_DYNAMIC_STORAGE_BIT);

		if (m_EBO != 0) {
			glDeleteBuffers(1, & m_EBO);
		}

		m_EBO = newEBO;
		m_IndexCapacity = newCapacity;

		if (m_VAO != 0) {
			glVertexArrayElementBuffer(m_VAO, m_EBO);
		}
	}
}
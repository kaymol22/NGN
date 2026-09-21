#include "MeshBuffer.h"
#include "Renderer.h"

namespace NGN::Renderer
{
	void MeshBuffer::Initialize() {
		if (Renderer::GetActiveAPI() == API::OPENGL) {
			if (m_OpenGLId == 0) {
				m_OpenGLId = OpenGL::ResourceManager::CreateMeshBuffer(m_Name);
			}
			OpenGL::ResourceManager::GetMeshBuffer(m_OpenGLId).Init(Vertex::GetLayout());
		}
		m_Initialized = true;
	}

	uint32_t MeshBuffer::AddMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t> indices, const std::string& name) {
		if (!m_Initialized) Initialize();
		if (vertices.empty() || indices.empty()) return 0;

		uint32_t meshId = ++m_NextMeshId;
		MeshDescriptor& desc = m_Meshes[meshId];
		desc.baseVertex = static_cast<uint32_t>(m_VertexCount);
		desc.baseIndex = static_cast<uint32_t>(m_IndexCount);
		desc.vertexCount = static_cast<uint32_t>(vertices.size());
		desc.indexCount = static_cast<uint32_t>(indices.size());
		desc.name = name;
		m_MeshIdByName[name] = meshId;

		glm::vec3 aabbMin(std::numeric_limits<float>::max());
		glm::vec3 aabbMax(std::numeric_limits<float>::lowest());
		for (const auto& vert : vertices) { aabbMin = glm::min(aabbMin, vert.position); aabbMax = glm::max(aabbMax, vert.position); }
		desc.aabbMin = aabbMin;
		desc.aabbMax = aabbMax;

		m_VertexCount += vertices.size();
		m_IndexCount += indices.size();

		if (Renderer::GetActiveAPI() == API::OPENGL) {
			OpenGL::OpenGLMeshBuffer& buf = OpenGL::ResourceManager::GetMeshBuffer(m_OpenGLId);
			buf.InsertVertices(vertices, desc.baseVertex);
			buf.InsertIndices(indices, desc.baseIndex);
		}

		return meshId;
	}
}
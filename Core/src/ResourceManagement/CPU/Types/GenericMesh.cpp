#include "GenericMesh.h"
#include "Core/Application.h"

#include "Render/API/OpenGL/GL_ResourceManager.h"
#include "Geometry/PrimitiveMesh.h"

namespace RS
{
	bool GenericMesh::doLoad() {
		if (GetId() == "DefaultCube") {
			if (!LoadCube())
			{
				NGN_CORE_ERROR("GenericMesh::doLoad() failed to build default cube for resource: {}", GetId());
				return false;
			}
		}
		else
		{
			NGN_CORE_ERROR("GenericMesh::doLoad() - unknown mesh id '{}'", GetId());
			return false;
		}

		m_VertexStride = sizeof(Vertex);
		m_VertexCount = m_Vertices.size();
		m_IndexCount = m_Indices.size();

		if (NGN::Application::Get().GetAPI() == API::OPENGL) {
			uint64_t id = OpenGL::ResourceManager::CreateGenericMesh(GetId());
			SetGLId(id);

			OpenGL::OpenGLGenericMesh* mesh = OpenGL::ResourceManager::GetGenericMeshPtrById(id);
			if (!mesh) {
				NGN_CORE_ERROR("GenericMesh::doLoad() - failed to retrieve OpenGLGenericMesh for '{}'", GetId());
				return false;
			}

			mesh->UpdateVertexData(m_Vertices.data(), m_Vertices.size(), Vertex::GetLayout());
			mesh->UpdateIndexData(m_Indices);
		}
		NGN_CORE_INFO("GenericMesh::doLoad() successful for '{}' ({} verts, {} indices)", GetId(), m_VertexCount, m_IndexCount);
		return true;
	}

	bool GenericMesh::doUnload() {
		m_Vertices.clear();
		m_Vertices.shrink_to_fit();
		m_Indices.clear();
		m_Indices.shrink_to_fit();

		m_VertexCount = 0;
		m_IndexCount = 0;
		m_VertexCapacity = 0;
		m_IndexCapacity = 0;

		if (NGN::Application::Get().GetAPI() == API::OPENGL) {
			OpenGL::ResourceManager::RemoveGenericMesh(m_OpenGLId);
		}
		return true;
	}

	bool GenericMesh::LoadCube() {
		m_Vertices = NGN::Geometry::GenerateCubeVertices();
		m_Indices = NGN::Geometry::GenerateCubeIndices();

		m_BoundsMin = glm::vec3(-0.5f);
		m_BoundsMax = glm::vec3( 0.5f);

		return !m_Vertices.empty() && !m_Indices.empty();
	}
}
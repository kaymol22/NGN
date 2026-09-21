#include "Mesh.h"
#include "Core/Application.h"
#include "Render/Renderer.h"
#include "Render/API/OpenGL/GL_ResourceManager.h"
#include "Geometry/PrimitiveMesh.h"

namespace RS
{
	bool MeshAsset::doLoad() {
		std::vector<RawSubMeshData> rawSubMeshes;

		if (GetId() == "DefaultCube") {
			if (!LoadCube(rawSubMeshes))
			{
				NGN_CORE_ERROR("GenericMesh::doLoad() failed to build default cube for resource: {}", GetId());
				return false;
			}
			m_BufferName = "AssetGeometry";
		}
		else
		{
			NGN_CORE_ERROR("GenericMesh::doLoad() - unknown mesh id '{}'", GetId());
			return false;
		}

		NGN::Renderer::MeshBuffer& meshBuffer = NGN::Renderer::GetMeshBuffer(m_BufferName);

		glm::vec3 aggregateMin(std::numeric_limits<float>::max());
		glm::vec3 aggregateMax(std::numeric_limits<float>::lowest());

		for (const RawSubMeshData& raw : rawSubMeshes) {
			uint32_t meshId = meshBuffer.AddMesh(raw.vertices, raw.indices, raw.name);
			if (meshId == 0) {
				NGN_CORE_ERROR("GenericMesh::doLoad() failed to add submesh '{}' to mesh buffer '{}'", raw.name, m_BufferName);
				return false;
			}

			SubMesh subMesh;
			subMesh.meshId = meshId;
			subMesh.materialSlot = 0;
			subMesh.blendMode = raw.blendMode;
			subMesh.localTransform = raw.localTransform;
			subMesh.parentIndex = raw.parentIndex;
			m_SubMeshes.push_back(subMesh);

			const MeshDescriptor& desc = meshBuffer.GetMesh(meshId);
			aggregateMin = glm::min(aggregateMin, desc.aabbMin);
			aggregateMax = glm::max(aggregateMax, desc.aabbMax);

			m_VertexCount += raw.vertices.size();
			m_IndexCount += raw.indices.size();
		}

		m_BoundsMin = aggregateMin;
		m_BoundsMax = aggregateMax;
		m_VertexStride = sizeof(Vertex);

		NGN_CORE_INFO("GenericMesh::doLoad() successful for '{}' ({} verts, {} indices)", GetId(), m_VertexCount, m_IndexCount);
		return true;
	}

	bool MeshAsset::doUnload() {
		m_SubMeshes.clear();
		m_SubMeshes.shrink_to_fit();
		m_VertexCount = 0;
		m_IndexCount = 0;
		m_VertexStride = 0;

		return true;
	}

	bool MeshAsset::LoadCube(std::vector<RawSubMeshData>& outSubMeshes) {
		RawSubMeshData raw;
		raw.vertices = NGN::Geometry::GenerateCubeVertices();
		raw.indices = NGN::Geometry::GenerateCubeIndices();
		raw.name = "DefaultCube";
		raw.localTransform = glm::mat4(1.0f);
		raw.parentIndex = -1;
		raw.blendMode = BlendingMode::DEFAULT;
		
		if (raw.vertices.empty() || raw.indices.empty()) {
			return false;
		}
		outSubMeshes.push_back(std::move(raw));
		return true;
	}
}
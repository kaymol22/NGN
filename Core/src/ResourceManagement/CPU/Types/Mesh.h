#pragma once

#include "Resource.h"
#include "File/FileInfo.h"
#include "Core/Enums.h"
#include "Render/VertexAttributes.h"
#include "SubMesh.h"

namespace RS
{
	class MeshAsset : public Resource
	{
	public:
		explicit MeshAsset(const std::string& id) : Resource(id) {}
		~MeshAsset() override { Unload(); }

		size_t GetVertexCount() const { return m_VertexCount; }
		size_t GetIndexCount() const { return m_IndexCount; }

		const glm::vec3& GetBoundsMin() const { return m_BoundsMin; }
		const glm::vec3 GetBoundsMax() const { return m_BoundsMax; }

		std::vector<SubMesh>& GetSubMeshes() { return m_SubMeshes; }
	protected:
		bool doLoad() override;
		bool doUnload() override;
	
	private:
		bool LoadCube(std::vector<RawSubMeshData>& rawSubMeshes);

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<SubMesh> m_SubMeshes;
		std::string m_BufferName;

		glm::vec3 m_BoundsMin = glm::vec3(-0.5f);
		glm::vec3 m_BoundsMax = glm::vec3(0.5f);
		
		size_t m_VertexCount = 0;
		size_t m_IndexCount = 0;
		size_t m_VertexStride = 0;
		
		FileInfo m_FileInfo;
	};
}
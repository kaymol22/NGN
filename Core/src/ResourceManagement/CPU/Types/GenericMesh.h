#pragma once

#include "Resource.h"
#include "File/FileInfo.h"
#include "Core/Enums.h"
#include "Render/VertexAttributes.h"

namespace RS
{
	class GenericMesh : public Resource
	{
	public:
		explicit GenericMesh(const std::string& id) : Resource(id) {}
		~GenericMesh() override { Unload(); }

		size_t GetVertexCount() const { return m_VertexCount; }
		size_t GetIndexCount() const { return m_IndexCount; }

		const glm::vec3& GetBoundsMin() const { return m_BoundsMin; }
		const glm::vec3 GetBoundsMax() const { return m_BoundsMax; }

		uint64_t GetGLId() { return m_OpenGLId; }
		void SetGLId(uint64_t id) { m_OpenGLId = id; }

	protected:
		bool doLoad() override;
		bool doUnload() override;
	
	private:
		bool LoadCube();

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		glm::vec3 m_BoundsMin = glm::vec3(-0.5f);
		glm::vec3 m_BoundsMax = glm::vec3(0.5f);
		
		size_t m_VertexStride = 0;
		size_t m_VertexCount = 0;
		size_t m_IndexCount = 0;
		size_t m_VertexCapacity = 0;
		size_t m_IndexCapacity = 0;
		
		uint64_t m_OpenGLId = 0;
		uint64_t m_VulkanId = 0;
	};
}
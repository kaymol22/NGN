#pragma once
#include "VertexAttributes.h"
#include "API/OpenGL/GL_ResourceManager.h"
#include "API/OpenGL/Types/GL_MeshBuffer.h"

namespace NGN::Renderer
{
	class MeshBuffer
	{
	public:
		explicit MeshBuffer(const std::string& name) : m_Name(name) {}

		void Initialize();

		uint32_t AddMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t> indices, const std::string& name);

		// Might need this for navmesh gen/ collisions
		// Accessor here to avoid duplicating to mesh resource
		void GetVertexRange(uint32_t meshId);

		const MeshDescriptor& GetMesh(uint32_t meshId) const { return m_Meshes.at(meshId); }
		uint64_t GetOpenGLId() const { return m_OpenGLId; }

	private:
		std::string m_Name;
		size_t m_VertexCount = 0, m_IndexCount = 0;
		uint64_t m_OpenGLId = 0;
		uint64_t m_VulkanId = 0;
		uint32_t m_NextMeshId = 0;
		bool m_Initialized = false;

		std::unordered_map<uint32_t, MeshDescriptor> m_Meshes;
		std::unordered_map<std::string, uint32_t> m_MeshIdByName;
	};
}
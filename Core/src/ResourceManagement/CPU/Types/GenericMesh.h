#pragma once

#include "Resource.h"
#include "File/FileInfo.h"
#include "Core/Enums.h"

namespace RS
{
	class GenericMesh : public Resource
	{
	public:
		explicit GenericMesh(const std::string& id) : Resource(id) {}
		~GenericMesh() override { Unload(); }

		uint64_t GetGLId() { return m_OpenGLId; }
		void SetGLId(uint64_t id) { m_OpenGLId = id; }

	protected:
		bool doLoad() override;
		bool doUnload() override;

	private:
		size_t m_VertexStride = 0;
		size_t m_VertexCount = 0;
		size_t m_IndexCount = 0;
		size_t m_VertexCapacity = 0;
		size_t m_IndexCapacity = 0;
		uint64_t m_OpenGLId = 0;
		uint64_t m_VulkanId = 0;
	};
}
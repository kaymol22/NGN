#pragma once
#include "Types/GL_Texture.h"
#include "Types/GL_ssbo.h"
#include "Containers/SlotMap.h"
#include "ResourceManagement/CPU/Types/Texture.h"

namespace OpenGL
{
	class OpenGLResourceManager
	{
	public:
		uint64_t GetOrCreateTexture(RS::Texture& cpuTexture);
		OpenGLTexture& GetTexture(uint64_t id);
		OpenGLTexture* GetTexturePtr(uint64_t id);
		void ReleaseTexture(uint64_t id);
		size_t GetAllocatedTextureCount() const;

		/*OpenGLSSBO& CreateSSBO(const std::string& name);
		OpenGLSSBO& GetSSBO(const std::string& name);
		OpenGLSSBO* GetSSBOPtr(const std::string& name);
		OpenGLSSBO* GetSSBOPtrById(uint64_t id);
		void RemoveSSBOByName(const std::string& name);
		void RemoveSSBO(uint64_t id);*/

	private:
		NGN::SlotMap<OpenGLTexture> m_Textures;
		NGN::SlotMap<OpenGLTexture> m_SSBOs;
		std::unordered_map<uint64_t, uint64_t> m_LastUsedFrame;
		uint64_t m_CurrentFrame;
	};
}
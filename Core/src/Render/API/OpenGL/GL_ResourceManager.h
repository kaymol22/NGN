#pragma once
#include "Types/GL_Texture.h"
#include "Containers/SlotMap.h"
#include "ResourceManagement/CPU/Types/Texture.h"

namespace OpenGL
{
	class OpenGLResourceManager
	{
	public:
		uint64_t CreateTexture(RS::Texture& cpuTexture);
		OpenGLTexture& GetTexture(uint64_t id);
		OpenGLTexture* GetTexturePtr(uint64_t id);


		void ReleaseTexture(uint64_t id);
		size_t GetAllocatedTextureCount() const;

	private:
		NGN::SlotMap<OpenGLTexture> m_Textures;
		std::unordered_map<uint64_t, uint64_t> m_LastUsedFrame;
		uint64_t m_CurrentFrame;
	};
}
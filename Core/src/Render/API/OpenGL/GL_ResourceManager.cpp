#include "GL_ResourceManager.h"

namespace OpenGL
{
	uint64_t OpenGLResourceManager::CreateTexture(RS::Texture& cpuTexture)
	{
		uint64_t id = cpuTexture.GetGLId();
		m_Textures.emplace_with_id(id);
		return id;
	}

	OpenGLTexture& OpenGLResourceManager::GetTexture(uint64_t id)
	{
		OpenGLTexture* texture = GetTexturePtr(id);
		if (texture)
			return *texture;
		static OpenGLTexture invalid;
		return invalid;
	}

	OpenGLTexture* OpenGLResourceManager::GetTexturePtr(uint64_t id)
	{
		return m_Textures.get(id);
	}
}
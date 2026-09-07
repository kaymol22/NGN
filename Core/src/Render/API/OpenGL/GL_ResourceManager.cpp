#include "GL_ResourceManager.h"

namespace OpenGL
{
	uint64_t OpenGLResourceManager::GetOrCreateTexture(RS::Texture& cpuTexture)
	{
		// Already uploaded
		uint64_t existingId = cpuTexture.GetGLId();
		if (existingId != 0 && m_Textures.contains(existingId))
			return existingId;

		// TODO: Implement better id assignments later
		uint64_t id = std::hash<std::string>{}(cpuTexture.GetId());

		if (!m_Textures.contains(id))
		{
			m_Textures.emplace_with_id(id);
		}

		OpenGLTexture* gpuTexture = m_Textures.get(id);
		if (!gpuTexture)
		{
			NGN_CORE_ERROR("OpenGLResourceManager::CreateTexture - failed to allocate GPU texture slot for '{}'", cpuTexture.GetId());
			return 0;
		}

		cpuTexture.SetGLId(id);
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

	void OpenGLResourceManager::ReleaseTexture(uint64_t id)
	{
		OpenGLTexture* texture = GetTexturePtr(id);
		if (texture)
			return texture->Reset();

		m_Textures.erase(id);
	}

	size_t OpenGLResourceManager::GetAllocatedTextureCount() const
	{
		return m_Textures.size();
	}

	/*OpenGLSSBO& OpenGLResourceManager::CreateSSBO(const std::string& name)
	{
		
	}*/
}
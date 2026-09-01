#include "ResourceManager.h"

namespace RS
{
	void ResourceManager::UnloadAll()
	{
		for (auto& [type, typeResources] : m_Resources)
		{
			for (auto& [id, data] : typeResources)
			{
				data.resource->Unload();
			}
			typeResources.clear();
		}
		m_Resources.clear();
	}
}
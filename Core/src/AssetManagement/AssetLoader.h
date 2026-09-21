#pragma once
#include "Core/Application.h"
#include "AssetManifest.h"
#include "ResourceManagement/CPU/ResourceManager.h"
#include "ResourceManagement/CPU/Types/Mesh.h"

namespace NGN
{
	class AssetLoader
	{
	public:
		static void LoadAll(RS::ResourceManager& resourceManager, const AssetManifest& manifest)
		{
			NGN_PROFILE_SCOPE("AssetLoader::LoadAll");
			
			for (const auto& entry : manifest.GetEntries())
			{
				switch (entry.type) {
					case AssetType::Texture:
						resourceManager.Load<RS::Texture>(entry.id);
						break;
					case AssetType::StaticMesh:
						resourceManager.Load<RS::MeshAsset>(entry.id);
						break;
				}
				NGN_CORE_INFO("Loaded asset: {} of type {}", entry.id, static_cast<int>(entry.type));
			}
		}
	};
}
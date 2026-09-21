#pragma once

namespace NGN
{
	enum class AssetType
	{
		Shader,
		Texture,
		StaticMesh,
		Material
	};
	
	struct AssetManifestEntry
	{
		AssetType type;
		std::string id;
	};

	class AssetManifest
	{
	public:
		void Add(AssetType type, const std::string& id) { m_Entries.push_back({ type, id }); }
		void Clear() { m_Entries.clear(); }

		const std::vector<AssetManifestEntry>& GetEntries() const { return m_Entries; }
	private:
		std::vector<AssetManifestEntry> m_Entries;
	};
}
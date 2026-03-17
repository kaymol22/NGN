#pragma once

#include <filesystem>

#include "Core/Base.h"
#include "Texture.h"

namespace NGN
{
	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& fontPath);
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }
		Ref<Texture2D> GetTexture() const { return m_Texture; }

	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_Texture;
	};

}
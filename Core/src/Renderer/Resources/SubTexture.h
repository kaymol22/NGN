#pragma once

#include "Texture.h"

#include "glm/glm.hpp"

namespace NGN
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, glm::vec2& min, glm::vec2& max);
		
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		const Ref<Texture2D>& GetTexture() const { return m_Texture; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize = {128, 128}, const glm::vec2& cellSize = {1,1});
		static Ref<SubTexture2D> CreateFromPixels(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& pixelSize);
	private:
		Ref<Texture2D> m_Texture;

		glm::vec2 m_TexCoords[4];
	};
}
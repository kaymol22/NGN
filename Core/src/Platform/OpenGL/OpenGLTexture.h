#pragma once

#include "Renderer/Texture.h"

namespace NGN
{
	class OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(const TextureSpecification& specification);
		OpenGLTexture(const std::string& path);

		virtual ~OpenGLTexture();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		uint32_t GetRenderID() const override { return m_RendererID; }
		const std::string& GetPath() const override { return m_Path; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Width = 0, m_Height = 0;
		std::string m_Path;
	};
}
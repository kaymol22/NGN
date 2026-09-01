#include "Core/Application.h"
#include "File/File.h"
#include "Texture.h"
#include "Resource.h"

#include "Render/API/OpenGL/GL_ResourceManager.h"

#include <stb_image.h>

namespace RS
{
	void Texture::SetTextureWrapMode(TextureWrapMode wrapS, TextureWrapMode wrapT)
	{
		m_WrapModeS = wrapS;
		m_WrapModeT = wrapT;
	}

	bool Texture::doLoad() {
		// id is texture name 
		std::filesystem::path fullPath = std::filesystem::path("res/textures") / (GetId() + ".png");

		m_FileInfo.path = fullPath.string();
		m_FileInfo.dir = fullPath.parent_path().string();
		m_FileInfo.name = fullPath.stem().string();
		m_FileInfo.ext = fullPath.extension().string();

		if (!LoadImageData(m_FileInfo.path))
		{
			NGN_CORE_ERROR("Texture: doLoad - failed to load image data for '{}'", m_FileInfo.path);
			return false;
		}
		NGN_CORE_INFO("Texture::doLoad() successful for file: {}", m_FileInfo.path);
		return true;
	}

	bool Texture::doUnload()
	{
		m_ImageData.mips.clear();
		m_ImageData.type = ImageDataType::UNDEFINED;
		m_ImageData.format = ImageFormat::UNDEFINED;
		m_ImageDataType = ImageDataType::UNDEFINED;
		m_UploadState = UploadState::NOT_REQUESTED;

		// TODO: Unload GPU side resources through backend RM

		return true;
	}

	bool Texture::LoadImageData(const std::string& filePath)
	{
		stbi_set_flip_vertically_on_load(1);
		int width = 0, height = 0, channels = 0;
		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

		if (!data)
		{
			NGN_CORE_ERROR("Texture::LoadImageData - stbi load failed for '{}': '{}'", filePath, stbi_failure_reason());
			return false;
		}

		ImageFormat format = ImageFormat::UNDEFINED;
		switch (channels)
		{
			case 1: format == ImageFormat::R8_UNORM; break;
			case 2: format == ImageFormat::RG8_UNORM; break;
			case 3: format == ImageFormat::RGB8_UNORM; break;
			case 4: format == ImageFormat::RGBA8_UNORM; break;
			default:
				NGN_CORE_ERROR("Texture::LoadImageData - unsupported channel count ({}) for '{}'", channels, filePath);
				stbi_image_free(data);
				return false;
		}

		TextureMip mip;
		mip.width = static_cast<uint32_t>(width);
		mip.height = static_cast<uint32_t>(height);

		size_t dataSize = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);
		mip.data.resize(dataSize);
		std::memcpy(mip.data.data(), data, dataSize);

		stbi_image_free(data);

		m_ImageData.type = ImageDataType::UNCOMPRESSED;
		m_ImageData.format = format;
		m_ImageData.mips.clear();
		m_ImageData.mips.push_back(std::move(mip));

		m_ImageDataType = ImageDataType::UNCOMPRESSED;

		return true;
	}

}
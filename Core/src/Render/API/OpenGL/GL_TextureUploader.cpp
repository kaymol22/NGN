#include "GL_TextureUploader.h"

namespace OpenGL::TextureUploader
{
	// TODO: Expand this namespace out later with upload queue - something like
	/*
	* struct UploadSlot { GLuint pbo; size_t capacity; Texture* texture; }
	*/
	namespace {
		size_t GetUploadMipCount(RS::Texture texture)
		{
			return texture.GetMipMapsRequested() ? texture.GetImageData().mips.size() : 1;
		}
	}

	bool ValidateTexture(RS::Texture& texture)
	{
		const ImageData& imageData = texture.GetImageData();

		if (imageData.mips.empty()) {
			NGN_CORE_ERROR("TextureUploader::ValidateTexture() failed because texture '{}' has no mip data", texture.GetId());
			return false;
		}

		if (imageData.format == ImageFormat::UNDEFINED)
		{
			NGN_CORE_ERROR("TextureUploader::ValidateTexture() - texture '{}' has undefined format", texture.GetId());
			return false;
		}

		const TextureMip& baseMip = imageData.mips[0];
		if (baseMip.width == 0 || baseMip.height == 0 || baseMip.data.empty())
		{
			NGN_CORE_ERROR("TextureUploader::ValidateTexture() - texture '{}' has invalid baseMip", texture.GetId());
			return false;
		}
		NGN_CORE_INFO("Texture validation for '{}' successful", texture.GetId());
		return true;
	}

	bool UploadToGPUTexture(RS::Texture& texture, OpenGLTexture& gpuTexture)
	{
		const ImageData& imageData = texture.GetImageData();
		const TextureMip& baseMip = imageData.mips[0];
		const bool generateMipMaps = texture.GetMipMapsRequested() && imageData.mips.size() == 1;
		const int allocatedMipCount = generateMipMaps ? texture.GetMipMapLevelCount() : static_cast<int>(GetUploadMipCount(texture));
		const GLenum internalFormat = Utils::ImageFormatToGLInternalFormat(imageData.format);
		if (internalFormat == 0)
		{
			NGN_CORE_ERROR("TextureUploader::CreateGPUTexture - undefined internal format for '{}'", texture.GetId());
			texture.SetUploadState(UploadState::FAILED);
			return false;
		}

		const int mipmapLevelCount = generateMipMaps
			? static_cast<int>(std::floor(std::log2(std::max(baseMip.width, baseMip.height)))) + 1
			: static_cast<int>(imageData.mips.size());
		
		gpuTexture.Create(static_cast<int>(baseMip.width), static_cast<int>(baseMip.height), internalFormat, mipmapLevelCount);

		if (IsCompressedImageFormat(imageData.format))
		{
			// TODO: Implement compreessed image uploads - need glCompressedTextureSubImage2D
			NGN_CORE_ERROR("TextureUploader::UploadToGPUTexture - compressed format not supported yet");
			texture.SetUploadState(UploadState::FAILED);
			return false;
		}

		const GLenum sourceType = GL_UNSIGNED_BYTE;

		for (size_t mipLevel = 0; mipLevel < imageData.mips.size(); ++mipLevel)
		{
			const TextureMip& mip = imageData.mips[mipLevel];
			if (mip.data.empty())
				continue;
			gpuTexture.UploadPixelData(mip.data.data(), mip.data.size(), sourceType, static_cast<int>(sourceType));
		}

		gpuTexture.SetWrapModeS(texture.GetTextureWrapModeS());
		gpuTexture.SetWrapModeT(texture.GetTextureWrapModeT());
		gpuTexture.SetMinFilter(texture.GetMinFilter());
		gpuTexture.SetMagFilter(texture.GetMagFilter());
		gpuTexture.SetBorderColor(texture.GetBorderColor());

		// If mips requested but only ase supplied in CPUTexture*
		if (generateMipMaps && imageData.mips.size() == 1 && mipmapLevelCount > 1)
		{
			gpuTexture.GenerateMipMaps();
		}

		texture.SetUploadState(UploadState::UPLOADED);
	}
}
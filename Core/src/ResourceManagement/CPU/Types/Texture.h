#pragma once

#include "Resource.h"
#include "Render/TextureTypes.h"
#include "File/FileInfo.h"
#include "Core/Enums.h"

#include "Render/API/OpenGL/Types/GL_Texture.h"

namespace RS
{
	class Texture : public Resource
	{
	public:
		explicit Texture(const std::string& id) : Resource(id) {}
		~Texture() override { Unload(); }

		const ImageData& GetImageData() const { return m_ImageData; }

		const ImageDataType GetImageDataType() const { return m_ImageDataType; }
		const TextureWrapMode GetTextureWrapModeS() const { return m_WrapModeS; }
		const TextureWrapMode GetTextureWrapModeT() const { return m_WrapModeT; }
		const TextureFilter GetMinFilter() const { return m_MinFilter; }
		const TextureFilter GetMagFilter() const { return m_MagFilter; }
		const glm::vec4& GetBorderColor() const { return m_BorderColor; }
		int GetMipMapLevelCount() const { return m_MipMapLevelCount; }
		bool GetMipMapsRequested() const { return m_MipMapsRequested; }
		UploadState GetUploadState() { return m_UploadState; }

		void SetTextureWrapMode(TextureWrapMode wrapS, TextureWrapMode wrapT);
		void SetUploadState(UploadState state) { m_UploadState = state; }

		uint64_t GetGLId() { return m_OpenGLId; }
		void SetGLId(uint64_t id) { m_OpenGLId = id; }

	protected:
		bool doLoad() override;
		bool doUnload() override;

	private:
		bool LoadImageData(const std::string& filePath);
	private:
		UploadState m_UploadState = UploadState::NOT_REQUESTED;
		ImageDataType m_ImageDataType = ImageDataType::UNDEFINED;
		TextureWrapMode m_WrapModeS = TextureWrapMode::REPEAT;
		TextureWrapMode m_WrapModeT = TextureWrapMode::REPEAT;
		TextureFilter m_MinFilter = TextureFilter::NEAREST;
		TextureFilter m_MagFilter = TextureFilter::NEAREST;
		FileInfo m_FileInfo;
		ImageData m_ImageData;
		int m_MipMapLevelCount = 0;
		bool m_MipMapsRequested = false;
		glm::vec4 m_BorderColor = glm::vec4(0.0f);

		uint64_t m_OpenGLId = 0;
		uint64_t m_VulkanId = 0;
		uint32_t m_BindlessIndex = -1;
	};
}
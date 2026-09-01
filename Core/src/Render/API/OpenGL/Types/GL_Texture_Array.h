#pragma once

namespace OpenGL
{
	struct TextureArray {
		void AllocateMemory(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t mipmapLevelCount, uint32_t count);
		void CleanUp();

	private:
		uint32_t m_Handle = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_Count = 0;
		uint32_t m_Format = 0;
		uint32_t m_InternalFormat = 0;
		uint32_t m_MipMapLevelCount = 0;
		uint32_t m_Type = 0;
		bool m_MemoryAllocated = false;
	};
}
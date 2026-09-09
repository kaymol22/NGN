#pragma once

namespace NGN
{
	class RenderBackend
	{
	public:
		struct Config {
			const size_t MAX_TEXTURE_WIDTH = 4096;
			const size_t MAX_TEXTURE_HEIGHT = 4096;
			const size_t MAX_CHANNEL_COUNT = 4;
			const size_t MAX_DATA_SIZE = MAX_TEXTURE_WIDTH * MAX_TEXTURE_HEIGHT * MAX_CHANNEL_COUNT;
		};
		RenderBackend() = default;
		virtual void Init(void* nativeWindowHandle) = 0;
		virtual void Shutdown() = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		const Config& GetConfig() const { return m_Config; }
	protected:
		Config m_Config;
	};
}
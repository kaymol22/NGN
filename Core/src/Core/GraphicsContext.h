#pragma once
#include "Types.h"
#include "Render/RenderBackend.h"

namespace NGN
{
	class GraphicsContext
	{
	public:
		GraphicsContext() = default;
		~GraphicsContext() = default;

		void SetAPI(API api);
		API GetAPI() const { return m_API; }
		
		// Create backend - GL Context / Vulkan device
		// Build rendergraph, configure resource limits
		void Init(void* nativeWindowHandle);
		void Shutdown();
		
		// TODO: Runtime swap of Graphics API's
		// Tear down current backend/rendergraph/resourcemanager
		// App needs to call to handle window recreation + callback setting
		void SwitchAPI(API newAPI);

		void BeginFrame();
		void Flush();
		void EndFrame();
		void OnWindowResize(uint32_t width, uint32_t height);
		void SwapBuffers();
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		RenderBackend& GetBackend() { return *m_Backend; }

	private:
		Scope<RenderBackend> m_Backend;
		API m_API = API::UNDEFINED;
		uint32_t maxCompressedTextureResolution = 0;
	};
}
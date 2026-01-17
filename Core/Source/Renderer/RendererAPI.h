#pragma once

namespace NGN
{
	enum class RendererAPIType
	{
		None = 0,
		OpenGL
		// Flexibility for additional API's
	};

	class RendererAPI
	{
	public:
		static RendererAPIType GetAPI() { return s_API; }
		static void SetAPI(RendererAPIType api) { s_API = api; }

	private:
		static RendererAPIType s_API;
	};
}
#include "GL_Rasterizer_State_Manager.h"

#include <unordered_map>

namespace OpenGL::RasterizerStateManager
{
	namespace {
		OpenGLRasterizerState g_GlobalState;
		bool g_StateInitialized = false;
		std::unordered_map<std::string, OpenGLRasterizerState> g_RasterizerStates;
	}

	OpenGLRasterizerState* CreateRasterizerState(const std::string& name) {
		g_RasterizerStates[name] = OpenGLRasterizerState();
		return &g_RasterizerStates[name];
	}

	OpenGLRasterizerState* GetRasterizerState(const std::string& name) {
		auto it = g_RasterizerStates.find(name);
		if (it == g_RasterizerStates.end()) {
			NGN_CORE_ERROR("OpenGL::RasterizerStateManager::GetRasterizerState() failed to get: ", name);
			return nullptr;
		}
		return &it->second;
	}
}
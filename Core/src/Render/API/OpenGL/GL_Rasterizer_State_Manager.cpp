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

	void ForceRasterizerState(const std::string& name) {
		OpenGLRasterizerState* rasterizerState = GetRasterizerState(name);
		if (!rasterizerState) {
			NGN_CORE_ERROR("RasterizerStateManager::ForceRasterizerState() failed - '{}' State doesn't exist", name);
			return;
		}
		ForceRasterizerState(*rasterizerState);
	}

	void ForceRasterizerState(const OpenGLRasterizerState& state) {
		state.Apply();
		g_GlobalState = state;
		g_StateInitialized = true;
	}
}
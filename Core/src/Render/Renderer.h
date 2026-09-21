#pragma once
#include "MeshBuffer.h"

namespace NGN::Renderer
{
	API GetActiveAPI();
	void Init(API api);
	void InitMain();
	void CleanUp();
	void RenderScene();
	void ClearTargets();
	void Shutdown();

	MeshBuffer& GetMeshBuffer(const std::string& name);
}
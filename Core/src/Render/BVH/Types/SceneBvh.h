#pragma once

#include "Render/BVH/Types.h"

struct SceneBvh
{
	std::vector<BvhNode> m_Nodes;
	std::vector<PrimitiveInstance> m_Instances;
	std::vector<GpuPrimitiveInstance> m_GpuInstances;
};
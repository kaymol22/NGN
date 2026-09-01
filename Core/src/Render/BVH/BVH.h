#pragma once

namespace NGN::Bvh
{
	uint64_t CreateSceneBvh();
	bool AddMeshToSceneBvh(uint64_t scenebvhId, uint64_t meshbvhId);
}
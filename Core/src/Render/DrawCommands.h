#pragma once

struct DrawIndexedIndirectCommand
{
	uint32_t indexCount = 0;
	uint32_t instanceCount = 0;
	uint32_t firstIndex = 0;
	int32_t baseVertex = 0;
	uint32_t baseInstance = 0; // Index into DrawRenderItemIndices
	void reset() {
		indexCount = 0;
		instanceCount = 0;
		firstIndex = 0;
		baseVertex = 0;
		baseInstance = 0;
	}
};

struct DrawCommandsSet
{
	std::vector<DrawIndexedIndirectCommand> standard;
	std::vector<DrawIndexedIndirectCommand> alphaDiscard;
};
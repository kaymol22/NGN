#pragma once 

#include "Core/Types.h"
#include "Renderer/Resources/Frustum.h"

#include <entt.hpp>

namespace NGN
{
	class SceneBVH
	{
	public:
		SceneBVH() = default;

		// Full rebuild - call on first load or when scene modified - iterates through whole registry
		void Build(entt::registry& registry);

		// Recompute AABB's for dirty primitives and refit affected nodes
		void UpdateDynamicBvhScene(entt::registry& registry);

		void MarkDirty(uint64_t objectID);

		// Populate out vector with entities within frustum
		void QueryFrustum(const Frustum& frustum, std::vector<entt::entity>& out);

		bool IsBuilt() const { return !m_Data.Nodes.empty(); }

		void DebugDraw();

	private:
		// Scratch data for first build - store primitive AABBs and centroids for sorting
		struct BuildContext
		{
			std::vector<glm::vec3> centroids;
			std::vector<glm::vec3> mins;
			std::vector<glm::vec3> maxs;

			void Reserve(uint32_t n) { centroids.reserve(n), mins.reserve(n), maxs.reserve(n); };
			void Clear() { centroids.clear(), mins.clear(), maxs.clear(); };
		};

		// Reads data from BuildContext
		void BuildRecursive(uint32_t nodeIndex, uint32_t* indices, uint32_t count);

		// SAH bucket split
		uint32_t FindBestSplit(uint32_t* indices, uint32_t count,
			const glm::vec3& parentMin, const glm::vec3& parentMax);

		void RefitNode(uint32_t nodeIndex);
		void QueryNode(uint32_t nodeIndex, const Frustum& frustum, std::vector<entt::entity>& out);

		static float SurfaceArea(const glm::vec3& min, const glm::vec3& max);

	private:
		static constexpr uint32_t k_BucketCount = 12;
		static constexpr uint32_t k_MaxPrimsPerLeaf = 4;

		SceneBvh m_Data;
		BuildContext BuildCtx;

		std::vector<uint32_t> m_PrimitiveIndices;
		std::vector<entt::entity> m_EntityHandles; // maps primitive index to entity
		// UUID -> primitive index - use for marking dirty primitives
		std::unordered_map<uint64_t, uint32_t> m_ObjectToPrimitive;
		std::unordered_set<uint64_t> m_DirtyObjects; // objects to update next frame
	};
}
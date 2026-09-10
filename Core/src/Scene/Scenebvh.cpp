#include "Scenebvh.h"
#include "Components.h"

// TODO: Look into bvh - madmann91 for future raytracing
// For now using frustum culling and octree for partitioning

namespace NGN
{
	void SceneBVH::Build(entt::registry& registry)
	{
		// Clear previous
		m_Data.Nodes.clear();
		m_Data.Primitives.clear();
		m_PrimitiveIndices.clear();
		m_EntityHandles.clear();
		m_ObjectToPrimitive.clear();
		m_DirtyObjects.clear();

		auto view = registry.view<TransformComponent, MeshComponent, IDComponent>();
		const uint32_t n = static_cast<uint32_t>(view.size());

		if (n == 0)
		{
			NGN_CORE_WARN("SceneBVH::Build - No entities with TransformComponent, MeshComponent, and IDComponent found.");
			return;
		}

		// To avoid vectors reallocating mid-fill**
		m_Data.Primitives.reserve(n);
		m_PrimitiveIndices.reserve(n);
		m_EntityHandles.reserve(n);
		BuildCtx.Reserve(n);

		// Fill prim data - one prim instance per mesh entity
		uint32_t slot = 0;
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& mesh = view.get<MeshComponent>(entity);
			auto& id = view.get<IDComponent>(entity);

			mesh.UpdateBounds(transform);

			PrimitiveInstance prim = {};
			prim.objectID = static_cast<uint64_t>(id.ID);
			prim.worldAabbBoundsMin = mesh.boundsMin;
			prim.worldAabbBoundsMax = mesh.boundsMax;
			prim.worldAabbCentre = (mesh.boundsMin + mesh.boundsMax) * 0.5f;
			prim.worldTransform = transform.GetTransform();
			prim.inverseWorldTransform = glm::inverse(prim.worldTransform);

			m_Data.Primitives.push_back(prim);
			m_EntityHandles.push_back(entity);
			m_ObjectToPrimitive[prim.objectID] = slot;

			BuildCtx.mins.push_back(mesh.boundsMin);
			BuildCtx.maxs.push_back(mesh.boundsMax);
			BuildCtx.centroids.push_back(prim.worldAabbCentre);

			m_PrimitiveIndices.push_back(slot);
			slot++;
		}

		// Then recursive build
		m_Data.Nodes.reserve(n * 2); // worst case
		m_Data.Nodes.emplace_back(); // set root to index 0

		BuildRecursive(0, m_PrimitiveIndices.data(), n);
		BuildCtx.Clear();

		NGN_CORE_INFO("SceneBVH built with {} nodes and {} primitives.", m_Data.Nodes.size(), m_Data.Primitives.size());
	}

	void SceneBVH::BuildRecursive(uint32_t nodeIndex, uint32_t* indices, uint32_t count)
	{
		// Compute AABB for node
		glm::vec3 nodeMin(1e30f), nodeMax(-1e30f);
		for (uint32_t i = 0; i < count; i++)
		{
			nodeMin = glm::min(nodeMin, BuildCtx.mins[indices[i]]);
			nodeMax = glm::max(nodeMax, BuildCtx.maxs[indices[i]]);
		}
		m_Data.Nodes[nodeIndex].aabbMin = nodeMin;
		m_Data.Nodes[nodeIndex].aabbMax = nodeMax;

		// Leaf node condition
		if (count <= k_MaxPrimsPerLeaf)
		{
			m_Data.Nodes[nodeIndex].firstChildOrPrimitive = 
				static_cast<uint32_t>(indices - m_PrimitiveIndices.data());
			m_Data.Nodes[nodeIndex].primitiveCount = count;
			return;
		}

		// Split with SAH - partitions indices 
		uint32_t splitPos = FindBestSplit(indices, count, nodeMin, nodeMax);

		// If all prims end up on one side make leaf**
		if (splitPos == ~0u || splitPos == 0 || splitPos == count)
		{
			m_Data.Nodes[nodeIndex].firstChildOrPrimitive =
				static_cast<uint32_t>(indices - m_PrimitiveIndices.data());
			m_Data.Nodes[nodeIndex].primitiveCount = count;
			return;
		}

		// Create child nodes
		uint32_t leftIndex = static_cast<uint32_t>(m_Data.Nodes.size());
		m_Data.Nodes.emplace_back(); // Left child 
		m_Data.Nodes.emplace_back(); // Right child == left + 1

		// primcount = 0, mark as internal node
		m_Data.Nodes[nodeIndex].firstChildOrPrimitive = leftIndex;
		m_Data.Nodes[nodeIndex].primitiveCount = 0;

		BuildRecursive(leftIndex, indices, splitPos);
		BuildRecursive(leftIndex + 1, indices + splitPos, count - splitPos);
	}

	// SAH (Surface Area Heuristic) Bucket split
	// Probability of a ray hit = surface area of child / surface area of parent
	// cost = (SA_left x n_left + SA_right x n_right) / SA_parent
	uint32_t SceneBVH::FindBestSplit(uint32_t* indices, uint32_t count,
		const glm::vec3& parentMin, const glm::vec3& parentMax)
	{
		const float parentSA = SurfaceArea(parentMin, parentMax);
		if (parentSA <= 0.0f)
			return ~0u;

		float bestCost = 1e30f;
		int bestAxis = -1;
		float bestSplitVal = 0.0f;

		for (int axis = 0; axis < 3; axis++)
		{
			float axisMin = 1e30f, axisMax = -1e30f;
			for (uint32_t i = 0; i < count; i++)
			{
				float c = BuildCtx.centroids[indices[i]][axis];
				axisMin = std::min(axisMin, c);
				axisMax = std::max(axisMax, c);
			}

			if (axisMax - axisMin < 1e-6f)
				continue;

			struct Bucket { glm::vec3 bMin{ 1e30f }; glm::vec3 bMax{ -1e30f }; uint32_t count = 0; };
			std::array<Bucket, k_BucketCount> buckets{};

			const float scale = static_cast<float>(k_BucketCount) / (axisMax - axisMin);
			for (uint32_t i = 0; i < count; i++)
			{
				uint32_t idx = indices[i];
				int b = static_cast<int>((BuildCtx.centroids[idx][axis] - axisMin) * scale);
				b = std::clamp(b, 0, static_cast<int>(k_BucketCount) - 1);

				buckets[b].bMin = glm::min(buckets[b].bMin, BuildCtx.mins[idx]);
				buckets[b].bMax = glm::max(buckets[b].bMax, BuildCtx.maxs[idx]);
				++buckets[b].count;
			}

			for (uint32_t s = 1; s < k_BucketCount; s++)
			{
				// Left
				glm::vec3 lMin(1e30f), lMax(-1e30f);
				uint32_t lCount = 0;

				for (uint32_t b = 0; b < s; b++)
				{
					if (buckets[b].count == 0)
						continue;
					lMin = glm::min(lMin, buckets[b].bMin);
					lMax = glm::max(lMax, buckets[b].bMax);
					lCount += buckets[b].count;
				}
				// Right
				glm::vec3 rMin(1e30f), rMax(-1e30f);
				uint32_t rCount = 0;
				for (uint32_t b = s; b < k_BucketCount; b++)
				{
					if (buckets[b].count == 0)
						continue;
					rMin = glm::min(rMin, buckets[b].bMin);
					rMax = glm::max(rMax, buckets[b].bMax);
					rCount += buckets[b].count;
				}

				if (lCount == 0 || rCount == 0)
					continue;

				float cost = (SurfaceArea(lMin, lMax) * static_cast<float>(lCount) +
					SurfaceArea(rMin, rMax) * static_cast<float>(rCount)) / parentSA;

				if (cost < bestCost)
				{
					bestCost = cost;
					bestAxis = axis;
					bestSplitVal = axisMin + (static_cast<float>(s) / k_BucketCount) * (axisMax - axisMin);
				}
			}
		}

		if (bestAxis < 0)
			return ~0u;

		auto* mid = std::partition(indices, indices + count, [&](uint32_t idx)
			{
				return BuildCtx.centroids[idx][bestAxis] < bestSplitVal;
			});

		return static_cast<uint32_t>(mid - indices);
	}

	void SceneBVH::QueryFrustum(const Frustum& frustum, std::vector<entt::entity>& out)
	{
		out.clear();
		if (m_Data.Nodes.empty())
			return;

		QueryNode(0, frustum, out);
	}

	void SceneBVH::QueryNode(uint32_t nodeIndex, const Frustum& frustum, std::vector<entt::entity>& out)
	{
		const BVHNode& node = m_Data.Nodes[nodeIndex];

		// If node AABB outside frustum, cull entire subtree
		if (!frustum.ContainsAABB(node.aabbMin, node.aabbMax))
			return;

		if (node.primitiveCount > 0)
		{
			// Leaf Node - add primitives to output
			for (uint32_t i = node.firstChildOrPrimitive;
				i < node.firstChildOrPrimitive + node.primitiveCount; i++)
			{
				out.push_back(m_EntityHandles[m_PrimitiveIndices[i]]);
			}
		}
		else
		{
			// Internal Node - recurse to children
			QueryNode(node.firstChildOrPrimitive, frustum, out);
			QueryNode(node.firstChildOrPrimitive + 1, frustum, out);
		}
	}

	void SceneBVH::UpdateDynamicBvhScene(entt::registry& registry)
	{
		if (m_DirtyObjects.empty())
			return;

		for (uint64_t objectID : m_DirtyObjects)
		{
			auto it = m_ObjectToPrimitive.find(objectID);
			if (it == m_ObjectToPrimitive.end())
			{
				// Entity has been added since last build - rebuild
				Build(registry);
				return;
			}

			uint32_t slot = it->second;
			entt::entity entity = m_EntityHandles[slot];
			auto* transform = registry.try_get<TransformComponent>(entity);
			auto* mesh = registry.try_get<MeshComponent>(entity);

			if (!transform || !mesh)
				continue;

			/*mesh->UpdateBounds(*transform);*/

			PrimitiveInstance& prim = m_Data.Primitives[slot];
			prim.worldAabbBoundsMin = mesh->boundsMin;
			prim.worldAabbBoundsMax = mesh->boundsMax;
			prim.worldAabbCentre = (mesh->boundsMin + mesh->boundsMax) * 0.5f;
			prim.worldTransform = transform->GetTransform();
			prim.inverseWorldTransform = glm::inverse(prim.worldTransform);
		}

		m_DirtyObjects.clear();

		// Bottom-up refit 
		RefitNode(0);
	}

	void SceneBVH::RefitNode(uint32_t nodeIndex)
	{
		for (int32_t i = static_cast<int32_t>(m_Data.Nodes.size()) - 1; i >= 0; i--)
		{
			BVHNode& node = m_Data.Nodes[i];

			// Leaf - recompute prim instances
			if (node.primitiveCount > 0)
			{
				node.aabbMin = glm::vec3(1e30f);
				node.aabbMax = glm::vec3(-1e30f);

				for (uint32_t j = node.firstChildOrPrimitive;
					j < node.firstChildOrPrimitive + node.primitiveCount; j++)
				{
					const PrimitiveInstance& prim = m_Data.Primitives[m_PrimitiveIndices[j]];
					node.aabbMin = glm::min(node.aabbMin, prim.worldAabbBoundsMin);
					node.aabbMax = glm::max(node.aabbMax, prim.worldAabbBoundsMax);
				}
			}
			// Internal - recompute children
			else
			{
				const BVHNode& left = m_Data.Nodes[node.firstChildOrPrimitive];
				const BVHNode& right = m_Data.Nodes[node.firstChildOrPrimitive + 1];
				node.aabbMin = glm::min(left.aabbMin, right.aabbMin);
				node.aabbMax = glm::max(left.aabbMax, right.aabbMax);
			}
		}
	}

	// ================================================================
	// Utils
	// ================================================================
	float SceneBVH::SurfaceArea(const glm::vec3& min, const glm::vec3& max)
	{
		glm::vec3 d = glm::max(max - min, glm::vec3(0.0f));
		return 2.0f * (d.x * d.y + d.y * d.z + d.z * d.x);
	}

	void SceneBVH::MarkDirty(uint64_t objectID)
	{
		m_DirtyObjects.insert(objectID);
	}

	void SceneBVH::DebugDraw()
	{

	}
}
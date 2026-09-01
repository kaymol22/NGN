#pragma once

#include <glm/glm.hpp>

namespace NGN
{
	struct FrustumPlane
	{
		glm::vec3 Normal;
		float Offset;
	};

	struct Frustum
	{
		Frustum() = default;

		void Update(const glm::mat4& viewProj);

		// Culling test
		bool ContainsAABB(const glm::vec3& min, const glm::vec3& max) const;

		const glm::vec3& GetBoundsMin() const { return m_Bounds[0]; }
		const glm::vec3& GetBoundsMax() const { return m_Bounds[1]; }
		const glm::vec4 GetPlane(int index);

		const glm::vec3& GetCorner(uint32_t index) const {
			NGN_CORE_ASSERT(index < 8); 
			return m_Corners[index]; 
		}

	private:
		glm::vec3 IntersectPlanes(const glm::vec3& n1, float d1, 
									const glm::vec3& n2, float d2, 
									const glm::vec3& n3, float d3) const;
		void ComputeBounds();
	private:
		FrustumPlane m_Planes[6] = {};
		glm::vec3 m_Corners[8] = {};
		glm::vec3 m_Bounds[2] = {};
	};
}
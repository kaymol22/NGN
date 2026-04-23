#include "Frustum.h"

#include "Frustum.h"
#include <glm/gtc/matrix_transform.hpp>

namespace NGN
{
	void Frustum::Update(const glm::mat4& viewProj)
	{
		// Left clipping plane
		m_Planes[0].Normal.x = viewProj[0][3] + viewProj[0][0];
		m_Planes[0].Normal.y = viewProj[1][3] + viewProj[1][0];
		m_Planes[0].Normal.z = viewProj[2][3] + viewProj[2][0];
		m_Planes[0].Offset   = viewProj[3][3] + viewProj[3][0];

		// Right clipping plane
		m_Planes[1].Normal.x = viewProj[0][3] - viewProj[0][0];
		m_Planes[1].Normal.y = viewProj[1][3] - viewProj[1][0];
		m_Planes[1].Normal.z = viewProj[2][3] - viewProj[2][0];
		m_Planes[1].Offset   = viewProj[3][3] - viewProj[3][0];

		// Top clipping plane
		m_Planes[2].Normal.x = viewProj[0][3] - viewProj[0][1];
		m_Planes[2].Normal.y = viewProj[1][3] - viewProj[1][1];
		m_Planes[2].Normal.z = viewProj[2][3] - viewProj[2][1];
		m_Planes[2].Offset   = viewProj[3][3] - viewProj[3][1];

		// Bottom clipping plane
		m_Planes[3].Normal.x = viewProj[0][3] + viewProj[0][1];
		m_Planes[3].Normal.y = viewProj[1][3] + viewProj[1][1];
		m_Planes[3].Normal.z = viewProj[2][3] + viewProj[2][1];
		m_Planes[3].Offset   = viewProj[3][3] + viewProj[3][1];

		// Near clipping plane
		m_Planes[4].Normal.x = viewProj[0][3] + viewProj[0][2];
		m_Planes[4].Normal.y = viewProj[1][3] + viewProj[1][2];
		m_Planes[4].Normal.z = viewProj[2][3] + viewProj[2][2];
		m_Planes[4].Offset   = viewProj[3][3] + viewProj[3][2];

		// Far clipping plane
		m_Planes[5].Normal.x = viewProj[0][3] - viewProj[0][2];
		m_Planes[5].Normal.y = viewProj[1][3] - viewProj[1][2];
		m_Planes[5].Normal.z = viewProj[2][3] - viewProj[2][2];
		m_Planes[5].Offset   = viewProj[3][3] - viewProj[3][2];

		// Normalize planes
		for (int i = 0; i < 6; i++)
		{
			float magnitude = glm::length(m_Planes[i].Normal);
			if (magnitude > 1e-6f)
			{
				m_Planes[i].Normal /= magnitude;
				m_Planes[i].Offset /= magnitude;
			}
		}

		// Compute frustum corners by checking plane intersection
		// 0=Left, 1=Right, 2=Top, 3=Bottom, 4=Near, 5=Far

		// Near bottom-left
		m_Corners[0] = IntersectPlanes(m_Planes[0].Normal, m_Planes[0].Offset, 
										 m_Planes[3].Normal, m_Planes[3].Offset, 
										 m_Planes[4].Normal, m_Planes[4].Offset);
		// Near bottom-right
		m_Corners[1] = IntersectPlanes(m_Planes[1].Normal, m_Planes[1].Offset, 
										 m_Planes[3].Normal, m_Planes[3].Offset, 
										 m_Planes[4].Normal, m_Planes[4].Offset);
		// Near top-left
		m_Corners[2] = IntersectPlanes(m_Planes[0].Normal, m_Planes[0].Offset, 
										 m_Planes[2].Normal, m_Planes[2].Offset, 
										 m_Planes[4].Normal, m_Planes[4].Offset);
		// Near top-right
		m_Corners[3] = IntersectPlanes(m_Planes[1].Normal, m_Planes[1].Offset, 
										 m_Planes[2].Normal, m_Planes[2].Offset, 
										 m_Planes[4].Normal, m_Planes[4].Offset);
		// Far bottom-left
		m_Corners[4] = IntersectPlanes(m_Planes[0].Normal, m_Planes[0].Offset, 
										 m_Planes[3].Normal, m_Planes[3].Offset, 
										 m_Planes[5].Normal, m_Planes[5].Offset); 
		// Far bottom-right
		m_Corners[5] = IntersectPlanes(m_Planes[1].Normal, m_Planes[1].Offset, 
										 m_Planes[3].Normal, m_Planes[3].Offset, 
										 m_Planes[5].Normal, m_Planes[5].Offset);
		// Far top-left
		m_Corners[6] = IntersectPlanes(m_Planes[0].Normal, m_Planes[0].Offset, 
										 m_Planes[2].Normal, m_Planes[2].Offset, 
										 m_Planes[5].Normal, m_Planes[5].Offset); 
		// Far top-right
		m_Corners[7] = IntersectPlanes(m_Planes[1].Normal, m_Planes[1].Offset, 
										 m_Planes[2].Normal, m_Planes[2].Offset, 
										 m_Planes[5].Normal, m_Planes[5].Offset); 

		// Compute AABB bounds from corners
		ComputeBounds();
	}

	glm::vec3 Frustum::IntersectPlanes(const glm::vec3& n1, float d1, 
										 const glm::vec3& n2, float d2, 
										 const glm::vec3& n3, float d3) const
	{
		// Solve system of 3 plane equations:
		// n1 · p + d1 = 0
		// n2 · p + d2 = 0
		// n3 · p + d3 = 0

		glm::vec3 crossN2N3 = glm::cross(n2, n3);
		float denom = glm::dot(n1, crossN2N3);

		// Check for parallel planes
		if (std::fabs(denom) < 1e-6f)
		{
			return glm::vec3(0.0f);
		}

		glm::vec3 result = -(d1 * crossN2N3 + d2 * glm::cross(n3, n1) + d3 * glm::cross(n1, n2)) / denom;
		return result;
	}

	void Frustum::ComputeBounds()
	{
		// Initialize bounds with first corner
		m_Bounds[0] = m_Corners[0];  // min
		m_Bounds[1] = m_Corners[0];  // max

		// Expand for all corners
		for (int i = 1; i < 8; i++)
		{
			m_Bounds[0] = glm::min(m_Bounds[0], m_Corners[i]);
			m_Bounds[1] = glm::max(m_Bounds[1], m_Corners[i]);
		}
	}

	const glm::vec4 Frustum::GetPlane(int index)
	{
		NGN_CORE_ASSERT(index >= 0 && index < 6, "Plane index out of bounds");
		return glm::vec4(m_Planes[index].Normal, m_Planes[index].Offset);
	}

	bool Frustum::ContainsAABB(const glm::vec3& min, const glm::vec3& max) const
	{
		// Test AABB against all 6 frustum planes
		// AABB is inside frustum if it's on the correct side of all planes

		for (int i = 0; i < 6; i++)
		{
			const FrustumPlane& plane = m_Planes[i];

			// Get the corner of the AABB that's closest to the plane normal
			glm::vec3 closestPoint = min;
			if (plane.Normal.x >= 0.0f) closestPoint.x = max.x;
			if (plane.Normal.y >= 0.0f) closestPoint.y = max.y;
			if (plane.Normal.z >= 0.0f) closestPoint.z = max.z;

			// Test if this point is behind the plane (outside frustum)
			float distance = glm::dot(plane.Normal, closestPoint) + plane.Offset;
			if (distance < 0.0f)
				return false;  // AABB is outside this plane
		}

		return true;  // AABB is inside all planes
	}
}

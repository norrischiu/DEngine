#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include "simdmath.h"
#include "Plane.h"
#include "Physics\cdSphere.h"
#include "Physics\cdAabb.h"

class Frustum
{
public:
	Frustum() {};
	Frustum(const float fFov, const float fRatio, const float fZNear, const float fZFar)
	{
		Reconstruct(fFov, fRatio, fZNear, fZFar);
	}

	// Bounding sphere culling
	bool Cull(Sphere sphere)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (!m_planes[i].IsInside(sphere))
			{
				return false;
			}
		}
		return true;
	}

	// Bounding box culling
	bool Cull(AABB box)
	{
		for (int i = 0; i < 6; ++i)
		{
			Vector3 planeNormal = m_planes[i].GetNormal();
			Vector3 max = box.getMax();
			Vector3 min = box.getMin();
			Vector3 testPoint
				(
					(planeNormal.GetX() > 0 ? max.GetX() : min.GetX()),
					(planeNormal.GetY() > 0 ? max.GetY() : min.GetY()),
					(planeNormal.GetZ() > 0 ? max.GetZ() : min.GetZ())
				);
			if (!m_planes[i].IsInside(testPoint))
			{
				return false;
			}
		}
		return true;
	}

private:

	void Reconstruct(const float fFov, const float fRatio, const float fZNear, const float fZFar)
	{
		Vector3 corners[8]; // clockwise from bottom left
							// top
		corners[0] = Vector3(-fZNear * tanf(fFov / 2.0f), fZNear * tanf(fFov / 2.0f), fZNear);
		corners[1] = Vector3(-fZFar * tanf(fFov / 2.0f), fZFar * tanf(fFov / 2.0f), fZFar);
		corners[2] = Vector3(fZFar * tanf(fFov / 2.0f), fZFar * tanf(fFov / 2.0f), fZFar);
		corners[3] = Vector3(fZNear * tanf(fFov / 2.0f), fZNear * tanf(fFov / 2.0f), fZNear);
		// bottom
		corners[4] = Vector3(-fZNear * tanf(fFov / 2.0f), -fZNear * tanf(fFov / 2.0f), fZNear);
		corners[5] = Vector3(-fZFar * tanf(fFov / 2.0f), -fZFar * tanf(fFov / 2.0f), fZFar);
		corners[6] = Vector3(fZFar * tanf(fFov / 2.0f), -fZFar * tanf(fFov / 2.0f), fZFar);
		corners[7] = Vector3(fZNear * tanf(fFov / 2.0f), -fZNear * tanf(fFov / 2.0f), fZNear);

		m_planes[0] = Plane(corners[3], corners[0], corners[4]); // near
		m_planes[1] = Plane(corners[1], corners[2], corners[6]);
		m_planes[2] = Plane(corners[0], corners[1], corners[5]); // left
		m_planes[3] = Plane(corners[2], corners[3], corners[7]);
		m_planes[4] = Plane(corners[3], corners[2], corners[1]); // top
		m_planes[5] = Plane(corners[4], corners[5], corners[6]);
	}

	Plane						m_planes[6];
};

#endif // !FRUSTUM_H_
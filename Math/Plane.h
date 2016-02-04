#ifndef PLANE_H_
#define PLANE_H_

#include "simdmath.h"
#include "Physics\cdSphere.h"
#include "Physics\cdAabb.h"

class Plane
{
public:

	Plane() {};

	// Points list in clockwise direction
	Plane(Vector3 a, Vector3 b, Vector3 c)
	{
		m_vNormal = Cross(b - a, c - b);
		m_vNormal.Normalize();
		m_fDist = a.Dot(m_vNormal);
	}

	bool IsInside(Sphere sphere)
	{
		float dist = m_vNormal.Dot(sphere.getCenter());
		return dist > -sphere.getRadius();
	}

	bool IsInside(Vector3 point)
	{
		float dist = m_vNormal.Dot(point);
		return dist > m_fDist;
	}

	inline Vector3 GetNormal()
	{
		return m_vNormal;
	}

private:

	Vector3						m_vNormal;
	float						m_fDist;
};

#endif // !PLANE_H_
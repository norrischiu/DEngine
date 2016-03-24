#ifndef PLANE_H_
#define PLANE_H_

#include "simdmath.h"
#include "Physics\cdSphere.h"
#include "Physics\cdAabb.h"

namespace DE
{

class Plane
{
public:

	Plane() {};

	// Points list in clockwise direction
	Plane(Vector3 a, Vector3 b, Vector3 c)
	{
		m_vNormal = Cross(c - a, c - b);
		float dist = a.Dot(m_vNormal);
		m_vNormal.Normalize();
		m_vNormal.SetW(dist);
	}

	bool IsInside(Sphere sphere)
	{
		float dist = m_vNormal.Dot(sphere.getCenter());
		return dist > -sphere.getRadius();
	}

	bool IsInside(Vector3 point)
	{
		float dist = m_vNormal.Dot(point);
		return dist > m_vNormal.GetW();
	}

	inline Vector3 GetNormal()
	{
		return m_vNormal;
	}

private:

	// Stores distance to w component
	Vector3						m_vNormal;
};

};
#endif // !PLANE_H_
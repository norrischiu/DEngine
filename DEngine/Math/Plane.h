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

	// Construct plane with 3 points (list in clockwise direction)
	Plane(Vector3 a, Vector3 b, Vector3 c)
	{
		m_vNormal = Cross(c - a, c - b);
		float length = m_vNormal.Length();
		m_vNormal.Normalize();
		m_vNormal.SetW(a.Dot(m_vNormal));
	}

	void Transform(Matrix4 transform)
	{
		Vector3 newPoint = m_vNormal * m_vNormal.GetW();
		newPoint.Transform(transform);
		m_vNormal.TransformAsVector(transform.Inverse().Transpose());
		m_vNormal.Normalize();
		m_vNormal.SetW(newPoint.Dot(m_vNormal));
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
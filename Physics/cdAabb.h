
#ifndef CDAABB_H
#define CDAABB_H

#include "Math/simdmath.h"
#include "cdCollide.h"
#include "cdSphere.h"
#include "cdBody.h"

class AABB : public Body
{
public:

	AABB(const Vector3& min, const Vector3& max)
	{
		m_Min = min;
		m_Max = max;
		Body::setType(typeAABB);
	}

	AABB(const Matrix4& transform, const Vector3& dimension, const Vector3& position);

	void computeAABB(const Vector3& position, const Vector3& dimension);

//	AABB(const SIMDVector3& dimension, const SIMDVector3& position);

	AABB() { Body::setType(typeAABB); }
	// check does two AABBs intersect? return an IntersectData
//	IntersectData collision(const Body& other);

	void Transform(Matrix4 transform)
	{
		Vector3 m_vMaxXYZ(FLT_MIN, FLT_MIN, FLT_MIN);
		Vector3 m_vMinXYZ(FLT_MAX, FLT_MAX, FLT_MAX);
		Vector3 corners[8];
		corners[0] = m_Min;
		corners[1] = Vector3(m_Min.GetX(), m_Min.GetY(), m_Max.GetZ());
		corners[2] = Vector3(m_Min.GetX(), m_Max.GetY(), m_Min.GetZ());
		corners[3] = Vector3(m_Max.GetX(), m_Min.GetY(), m_Max.GetZ());
		corners[4] = Vector3(m_Max.GetX(), m_Max.GetY(), m_Min.GetZ());
		corners[5] = Vector3(m_Max.GetX(), m_Min.GetY(), m_Max.GetZ());
		corners[6] = Vector3(m_Min.GetX(), m_Max.GetY(), m_Max.GetZ());
		corners[7] = m_Max;

		for (int i = 0; i < 8; ++i)
		{
			corners[i].Transform(transform);
			m_vMaxXYZ.SetX(corners[i].GetX() > m_vMaxXYZ.GetX() ? corners[i].GetX() : m_vMaxXYZ.GetX());
			m_vMaxXYZ.SetY(corners[i].GetY() > m_vMaxXYZ.GetY() ? corners[i].GetY() : m_vMaxXYZ.GetY());
			m_vMaxXYZ.SetZ(corners[i].GetZ() > m_vMaxXYZ.GetZ() ? corners[i].GetZ() : m_vMaxXYZ.GetZ());
			m_vMinXYZ.SetX(corners[i].GetX() < m_vMinXYZ.GetX() ? corners[i].GetX() : m_vMinXYZ.GetX());
			m_vMinXYZ.SetY(corners[i].GetY() < m_vMinXYZ.GetY() ? corners[i].GetY() : m_vMinXYZ.GetY());
			m_vMinXYZ.SetZ(corners[i].GetZ() < m_vMinXYZ.GetZ() ? corners[i].GetZ() : m_vMinXYZ.GetZ());
		}
		m_Min = m_vMinXYZ;
		m_Max = m_vMaxXYZ;
	}

	// two basic getters, return the max and min vector
	Vector3 getMin() const { return m_Min; }
	Vector3 getMax() const { return m_Max; }
	
	void setMin(const Vector3& min);

	void setMax(const Vector3& max);

	const Vector3 getCenter();

	virtual void update(const float deltaTime, const Vector3& translate);


private:
	// a vector which contains the minimum point
	Vector3			m_Min;
	// a vector which contains the maximum point
	Vector3			m_Max;
};


#endif

#ifndef CDAABB_H
#define CDAABB_H

#include "../Math/simdmath.h"
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
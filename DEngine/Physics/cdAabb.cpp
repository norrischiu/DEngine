#include "cdAabb.h"

AABB::AABB(const Matrix4 & transform, const Vector3 & dimension, const Vector3 & origin)
{
	float aabb[6];
	aabb[0] = origin.GetX() + dimension.GetX() / 2;
	aabb[1] = origin.GetY() - dimension.GetY() / 2;
	aabb[2] = origin.GetZ() + dimension.GetZ() / 2;
	aabb[3] = origin.GetX() - dimension.GetX() / 2;
	aabb[4] = origin.GetY() + dimension.GetY() / 2;
	aabb[5] = origin.GetZ() - dimension.GetZ() / 2;

	Vector3 min(aabb[0], aabb[1], aabb[2]);
	Vector3 max(aabb[3], aabb[4], aabb[5]);
	Vector3 translate(0.0f, 0.0f, 0.0f);
	translate.Transform(transform);
	min += translate;
	max += translate;

	m_Max = max;
	m_Min = min;
	Body::setType(1);
}


void AABB::computeAABB(const Vector3& position, const Vector3& dimension)
{
	float aabb[6];
	aabb[0] = position.GetX() + dimension.GetX() / 2;
	aabb[1] = position.GetY() - dimension.GetY() / 2;
	aabb[2] = position.GetZ() + dimension.GetZ() / 2;
	aabb[3] = position.GetX() - dimension.GetX() / 2;
	aabb[4] = position.GetY() + dimension.GetY() / 2;
	aabb[5] = position.GetZ() - dimension.GetZ() / 2;

	SIMDVector3 min(aabb[0], aabb[1], aabb[2]);
	SIMDVector3 max(aabb[3], aabb[4], aabb[5]);

	m_Max = max;
	m_Min = min;
}

void AABB::setMin(const Vector3 & min)
{
	m_Min = min;
}

void AABB::setMax(const Vector3 & max)
{
	m_Max = max;
}

const Vector3 AABB::getCenter()
{
	Vector3 m_pCenter = m_Min + m_Max;
	m_pCenter.Set(m_pCenter.GetX() / 2, m_pCenter.GetY() / 2,
		m_pCenter.GetZ() / 2);

	return m_pCenter;
}

void AABB::update(const float deltaTime, const Vector3 & translate)
{
	Vector3 tran = translate;
	tran.Multiply(deltaTime);
	m_Max += tran;
	m_Min += tran;
}




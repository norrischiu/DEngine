// SQT.h: scale-quaternion-translate representation
#ifndef SQT_H_
#define SQT_H_

#include "simdmath.h"

namespace DE
{

__declspec(align(16)) struct SQT
{
	SQT() {};

	SQT(const SIMDQuaternion& quat, const SIMDVector3& trans, const float& scale)
		: m_qQuat(quat)
		, m_vTrans(trans)
		, m_fScale(scale)
	{}

	SIMDQuaternion					m_qQuat;
	SIMDVector3						m_vTrans;
	float							m_fScale;

	SIMDMatrix4 Matrix()
	{
		SIMDMatrix4 result = m_qQuat.GetRotationMatrix();
		SIMDMatrix4 scale;
		scale.CreateScale(m_fScale);
		result *= scale;
		result.SetPosition(m_vTrans);
		return result;
	}

	static SQT LerpSQT(const SQT& a, const SQT& b, float t)
	{
		SQT result;
		result.m_fScale = a.m_fScale * (1 - t) + b.m_fScale * t;
		Quaternion fixedA = a.m_qQuat;
		if (a.m_qQuat.Dot(b.m_qQuat) < 0)
		{
			fixedA = -fixedA;
		}
		result.m_qQuat = Quaternion::Lerp(fixedA, b.m_qQuat, t);
		result.m_qQuat.Normalize();
		result.m_vTrans = Vector3::Lerp(a.m_vTrans, b.m_vTrans, t);
		return result;
	}

};

};

#endif // !SQT_H_

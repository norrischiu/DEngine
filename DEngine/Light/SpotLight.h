// SpotLight.h:
/*
#ifndef SPOTLIGHT_H_
#define SPOTLIGHT_H_

#include "Math/simdmath.h"
#include "LightManager.h"

class SpotLight
{
public:

	SpotLight() {};

	// Overload constructor: pos, r, amb, dif, spe, int
	SpotLight(Vector3 position, float radius, Vector4 ambient, Vector4 diffuse, Vector4 specular, float intensity)
		: m_vPos(position)
		, m_fAngle(radius)
		, m_vAmbient(ambient)
		, m_vDiffuse(diffuse)
		, m_vSpecular(specular)
		, m_fIntesity(intensity)
	{
		//m_fAngle = std::sqrt(256.0f * intensity);
		LightManager::GetInstance()->AddLight(this);
	};

	// Return radius, should be calculated with attenuation later
	float GetRadius()
	{
		return m_fAngle;
	}

	Vector3 GetPosition()
	{
		return m_vPos;
	}

	void SetPosition(Vector3 pos)
	{
		m_vPos = pos;
	}

private:
	Vector3								m_vPos;
	Vector4								m_vAmbient;
	Vector4								m_vDiffuse;
	Vector4								m_vSpecular;
	float								m_fAngle;
	float								m_fIntesity;
};

#endif
*/
#ifndef LIGHT_COMPONENT_H_
#define LIGHT_COMPONENT_H_

#include "Object\Component.h"
#include "Math\simdmath.h"

class LightComponent : public Component
{

public:

	enum eLightType
	{
		DIRECTIONAL,
		POINT,
		SPOT
	};

	LightComponent(Vector3 pos, Vector3 dir, Vector4 color, float radius, float intensity, int type, bool castShadow = false);

	// Inherited via Component
	virtual void Update(float deltaTime) override = 0;

	// Return radius, should be calculated with attenuation later
	inline float GetRadius()
	{
		return m_fRadius;
	}

	inline Vector3 GetPosition()
	{
		return m_vPos;
	}

	inline Vector3 GetDirection()
	{
		return m_vDirection;
	}

	inline float GetIntensity()
	{
		return m_fIntesity;
	}

	inline Vector4 GetColor()
	{
		return m_vColor;
	}

	inline int GetType()
	{
		return m_Type;
	}

	inline bool IsCastShadow()
	{
		return m_bCastShadow;
	}

	inline int GetShadowMapIndex()
	{
		return m_iShadowMapIndex;
	}

private:

	Vector3								m_vPos;
	Vector3								m_vDirection;
	Vector4								m_vColor;
	float								m_fRadius;
	float								m_fIntesity;
	int									m_Type;
	bool								m_bCastShadow;
	int									m_iShadowMapIndex;
};

#endif // !LIGHT_COMPONENT_H_

// PointLightComponent.h: the component of point light
#ifndef SPOTLIGHT_COMPONENT_H_
#define SPOTLIGHT_COMPONENT_H_

#include "GameObject\GameObject.h"
#include "Math\simdmath.h"
#include "LightComponent.h"
#include "LightManager.h"

namespace DE
{

class SpotLightComponent : public LightComponent
{

public:

	static const int ComponentID = ComponentID::SPOTLIGHT;

	SpotLightComponent(Vector3 pos, Vector3 dir, float innerAngle, float outerAngle, Vector4 color, float radius, float intensity, bool castShadow = false)
		: LightComponent(pos, dir, color, radius, intensity, eLightType::SPOT, castShadow)
		, m_fInnerAngle(innerAngle)
		, m_fOuterAngle(outerAngle)
	{
		LightManager::GetInstance()->AddLight(this);
		m_ID = ComponentID;
	};

	// Inherited via Component
	void Update(float deltaTime) override {};

	inline float GetInnerAngle()
	{
		return m_fInnerAngle;
	}

	inline float GetOuterAngle()
	{
		return m_fOuterAngle;
	}


	~SpotLightComponent() {};

private:

	float								m_fInnerAngle;
	float								m_fOuterAngle;

};

};

#endif // !SPOTLIGHT_COMPONENT_H_
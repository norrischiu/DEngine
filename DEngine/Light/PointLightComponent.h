// PointLightComponent.h: the component of point light
#ifndef POINTLIGHT_COMPONENT_H_
#define POINTLIGHT_COMPONENT_H_

#include "GameObject\GameObject.h"
#include "Math\simdmath.h"
#include "LightManager.h"
#include "LightComponent.h"

namespace DE
{

class PointLightComponent : public LightComponent
{

public:

	static const int ComponentID = ComponentID::POINTLIGHT;

	PointLightComponent(Vector3 pos, Vector4 color, float radius, float intensity, bool castShadow = false)
		: LightComponent(pos, Vector3::Zero, color, radius, intensity, eLightType::POINT, castShadow)
	{
		//m_fRadius = std::sqrt(256.0f * intensity);
		LightManager::GetInstance()->AddLight(this);
		m_ID = ComponentID;
	};

	// Inherited via Component
	void Update(float deltaTime) override 
	{
		// temp
		//m_vPos = Vector3::Zero;
		//m_vPos.Transform(*m_pOwner->GetTransform());
	};

	~PointLightComponent() {};

private:

};

};
#endif //!POINTLIGHT_COMPONENT_H_
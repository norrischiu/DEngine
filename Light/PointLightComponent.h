// PointLightComponent.h: the component of point light
#ifndef POINTLIGHT_COMPONENT_H_
#define POINTLIGHT_COMPONENT_H_

#include "Object\Component.h"
#include "GameObject\GameObject.h"
#include "Math\simdmath.h"
#include "LightManager.h"

class PointLightComponent : public Component
{

public:

	static const int ComponentID = ComponentID::POINTLIGHT;

	PointLightComponent() {};

	PointLightComponent(Vector3 position, float radius, Vector4 ambient, Vector4 diffuse, Vector4 specular, float intensity)
		: Component()
		, m_vPos(position)
		, m_fRadius(radius)
		, m_vAmbient(ambient)
		, m_vDiffuse(diffuse)
		, m_vSpecular(specular)
		, m_fIntesity(intensity)
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

	// Return radius, should be calculated with attenuation later
	float GetRadius()
	{
		return m_fRadius;
	}

	Vector3 GetPosition()
	{
		return m_vPos;
	}

	inline Vector4 GetDiffuse()
	{
		return m_vDiffuse;
	}

	inline Vector3 GetAmbient()
	{
		return m_vAmbient;
	}

	inline float GetIntensity()
	{
		return m_fIntesity;
	}

	void SetPosition(Vector3 pos)
	{
		m_vPos = pos;
	}

	~PointLightComponent() {};

private:

	Vector3								m_vPos;
	Vector4								m_vAmbient;
	Vector4								m_vDiffuse;
	Vector4								m_vSpecular;
	float								m_fRadius;
	float								m_fIntesity;


public:

};

#endif //!POINTLIGHT_COMPONENT_H_
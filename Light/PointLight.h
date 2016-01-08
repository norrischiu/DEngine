// PointLight.h: Wrapper as a game object by default has a single point light component
#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "Math/simdmath.h"
#include "GameObject/GameObject.h"
#include "PointLightComponent.h"
#include <cmath>

class PointLight : public GameObject
{

public:

	PointLight(Vector3 position, float radius, Vector4 ambient, Vector4 diffuse, Vector4 specular, float intensity)
	{
		AddComponent(new PointLightComponent(position, radius, ambient, diffuse, specular, intensity));
	}

	// Return radius, should be calculated with attenuation later
	float GetRadius()
	{
		return GetComponent<PointLightComponent>()->GetRadius();
	}

	Vector3 GetPosition()
	{
		return GetComponent<PointLightComponent>()->GetPosition();
	}

	/*
	void SetPosition(Vector3 pos)
	{
		return GetComponent<PointLightComponent>()->SetPosition(pos);
	}
	*/

};

#endif //!POINTLIGHT_H_
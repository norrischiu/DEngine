#ifndef LIGHTMANAGER_H_
#define LIGHTMANAGER_H_

#define MAX_LIGHT 100

#include <vector>
#include "LightComponent.h"
#include "Math\simdmath.h"
#include "Graphics\Render\Texture.h"

namespace DE
{

class LightManager
{
public:

	LightManager();

	static LightManager* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new LightManager();
		return m_pInstance;
	}

	void AddLight(LightComponent* light);

	int CreateShadowMap();

	LightComponent* GetLightAt(int index);

	inline int GetNumLights()
	{
		return m_iNumLight;
	}

	void* GetPointLightVertices()
	{
		return m_PointLightVertices;
	}

	void* GetSpotLightVertices()
	{
		return m_SpotLightVertices;
	}

	void* GetPointLightIndices()
	{
		return m_PointLightIndices;
	}

	void* GetSpotLightIndices()
	{
		return m_SpotLightIndices;
	}

	Texture* GetShadowMap(int index)
	{
		return m_ShadowMaps[index];
	}

private:

	// Singleton instance
	static LightManager*							m_pInstance;

	// Storage of all lights
	std::vector<LightComponent*>					m_vLights;

	// Storage of all shadow maps
	std::vector<Texture*>							m_ShadowMaps;

	// Number of lights
	unsigned int									m_iNumLight;

	Vector3*										m_PointLightVertices;
	unsigned int									m_PointLightIndices[36];

	Vector3*										m_SpotLightVertices;
	unsigned int									m_SpotLightIndices[18];

};

};
#endif
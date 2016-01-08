#ifndef LIGHTMANAGER_H_
#define LIGHTMANAGER_H_

#define MAX_LIGHT 100

#include <vector>
#include "Debug\debug.h"

class PointLightComponent;

class LightManager
{
public:

	LightManager()
		: m_iNumLight(0)
	{
		Debug geometry;
		Vertex1P* pVertices = new Vertex1P[8];
		UINT pIndices[36] =
		{ 0, 1, 2,    // side 1
			2, 1, 3,
			4, 0, 6,    // side 2
			6, 0, 2,
			7, 5, 6,    // side 3
			6, 5, 4,
			3, 1, 7,    // side 4
			7, 1, 5,
			4, 5, 0,    // side 5
			0, 5, 1,
			3, 7, 2,    // side 6
			2, 7, 6};
		pVertices[0].m_pos = Vector3(-0.5, 0.5, -0.5);
		pVertices[1].m_pos = Vector3(0.5, 0.5, -0.5);
		pVertices[2].m_pos = Vector3(-0.5, -0.5, -0.5);
		pVertices[3].m_pos = Vector3(0.5, -0.5, -0.5);
		pVertices[4].m_pos = Vector3(-0.5, 0.5, 0.5);
		pVertices[5].m_pos = Vector3(0.5, 0.5, 0.5);
		pVertices[6].m_pos = Vector3(-0.5, -0.5, 0.5);
		pVertices[7].m_pos = Vector3(0.5, -0.5, 0.5);
		m_PointLightVertices = pVertices;
		m_PointLightIndices = pIndices;
		//m_PointLightVertices = geometry.CreateUnitSphereVertex();
		//m_PointLightIndices = geometry.CreateUnitSphereIndex();
	};

	static LightManager* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new LightManager();
		return m_pInstance;
	}

	void AddLight(PointLightComponent* light);

	PointLightComponent* GetLightAt(int index);

	inline int GetNumLights()
	{
		return m_iNumLight;
	}

	void* GetPointLightVertices()
	{
		return m_PointLightVertices;
	}	
	
	void* GetPointLightIndices()
	{
		return m_PointLightIndices;
	}

private:

	// Singleton instance
	static LightManager*							m_pInstance;

	// Storage of all lights
	std::vector<PointLightComponent*>				m_vLights;

	// Number of lights
	unsigned int									m_iNumLight;

	Vertex1P*										m_PointLightVertices;
	unsigned int*									m_PointLightIndices;

};

#endif
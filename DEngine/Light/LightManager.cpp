#include "LightManager.h"
#include "PointLight.h"

namespace DE
{

LightManager* LightManager::m_pInstance;

LightManager::LightManager()
	: m_iNumLight(0)
{
	//Debug geometry;
	Vector3* pPointLightVertices = new Vector3[8];
	Vector3* pSpotLightVertices = new Vector3[5];
	unsigned int pPointLightIndices[36] =
	{
		0, 1, 2, // side 1
		2, 1, 3,
		4, 0, 6, // side 2
		6, 0, 2,
		7, 5, 6, // side 3
		6, 5, 4,
		3, 1, 7, // side 4
		7, 1, 5,
		4, 5, 0, // side 5
		0, 5, 1,
		3, 7, 2, // side 6
		2, 7, 6
	};
	pPointLightVertices[0] = Vector3(-1.0, 1.0, -1.0);
	pPointLightVertices[1] = Vector3(1.0, 1.0, -1.0);
	pPointLightVertices[2] = Vector3(-1.0, -1.0, -1.0);
	pPointLightVertices[3] = Vector3(1.0, -1.0, -1.0);
	pPointLightVertices[4] = Vector3(-1.0, 1.0, 1.0);
	pPointLightVertices[5] = Vector3(1.0, 1.0, 1.0);
	pPointLightVertices[6] = Vector3(-1.0, -1.0, 1.0);
	pPointLightVertices[7] = Vector3(1.0, -1.0, 1.0);
	m_PointLightVertices = pPointLightVertices;
	for (int i = 0; i < 36; ++i)
	{
		m_PointLightIndices[i] = pPointLightIndices[i];
	}

	unsigned int pSpotLightIndices[18] =
	{
		0, 1, 2, // top
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
		1, 4, 3, // base
		3, 2, 1,
	};
	pSpotLightVertices[0] = Vector3(0.0f, 1.0f, 0.0f);
	pSpotLightVertices[1] = Vector3(-1.0f, -1.0f, -1.0f);
	pSpotLightVertices[2] = Vector3(-1.0f, -1.0f, 1.0f);
	pSpotLightVertices[3] = Vector3(1.0f, -1.0f, 1.0f);
	pSpotLightVertices[4] = Vector3(1.0f, -1.0f, -1.0f);
	m_SpotLightVertices = pSpotLightVertices;
	for (int i = 0; i < 18; ++i)
	{
		m_SpotLightIndices[i] = pSpotLightIndices[i];
	}
}

void LightManager::AddLight(LightComponent* light)
{
	m_vLights.push_back(light);
	m_iNumLight++;
}

int LightManager::CreateShadowMap()
{
	Handle hTexture(sizeof(Texture));
	new (hTexture) Texture(Texture::DEPTH_STENCIL | Texture::SHADER_RESOURCES);
	m_ShadowMaps.push_back((Texture*) hTexture.Raw());
	return m_ShadowMaps.size() - 1;
}

LightComponent* LightManager::GetLightAt(int index)
{
	return m_vLights[index];
}

};
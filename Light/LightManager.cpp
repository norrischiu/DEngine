#include "LightManager.h"
#include "PointLight.h"

LightManager* LightManager::m_pInstance;

void LightManager::AddLight(PointLight* light)
{
	m_vLights.push_back(light);
	m_iNumLight++;
}

PointLight* LightManager::GetLightAt(int index)
{
	return m_vLights[index];
}
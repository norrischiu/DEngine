#include "LightManager.h"
#include "PointLight.h"

LightManager* LightManager::m_pInstance;

void LightManager::AddLight(PointLightComponent* light)
{
	m_vLights.push_back(light);
	m_iNumLight++;
}

PointLightComponent* LightManager::GetLightAt(int index)
{
	return m_vLights[index];
}
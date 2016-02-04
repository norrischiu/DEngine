#include "LightComponent.h"
#include "LightManager.h"

LightComponent::LightComponent(Vector3 pos, Vector3 dir, Vector4 color, float radius, float intensity, int type, bool castShadow)
	: m_vPos(pos)
	, m_vDirection(dir)
	, m_vColor(color)
	, m_fRadius(radius)
	, m_Type(type)
	, m_fIntesity(intensity)
	, m_bCastShadow(castShadow)
{
	if (castShadow)
	{
		m_iShadowMapIndex = LightManager::GetInstance()->CreateShadowMap();
	}
}

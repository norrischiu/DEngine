// DirectionalLight.h:

#include "Math/simdmath.h"

class DirectionalLight
{
public:

	DirectionalLight() {};

	// Overload constructor: dir, amb, dif, spe, int
	DirectionalLight(Vector3 direction, Vector4 ambient, Vector4 diffuse, Vector4 specular, float intensity)
		: m_vDirection(direction)
		, m_vAmbient(ambient)
		, m_vDiffuse(diffuse)
		, m_vSpecular(specular)
		, m_fIntesity(intensity)
	{};

private:
	Vector3								m_vDirection;
	Vector4								m_vAmbient;
	Vector4								m_vDiffuse;
	Vector4								m_vSpecular;
	float								m_fIntesity;
};

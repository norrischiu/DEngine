// PointLight.h:

#include <DirectXMath.h>
#include "../Math/simdmath.h"

typedef SIMDVector3 Vector3;
typedef SIMDVector3 Vector4;

class PointLight
{
public:

	PointLight() {};

	// Overload constructor: pos, r, amb, dif, spe, int
	PointLight(Vector3 position, float radius, Vector4 ambient, Vector4 diffuse, Vector4 specular, float intensity)
		: m_vPos(position)
		, m_fRadius(radius)
		, m_vAmbient(ambient)
		, m_vDiffuse(diffuse)
		, m_vSpecular(specular)
		, m_fIntesity(intensity)
	{};

private:
	Vector3								m_vPos;
	Vector4								m_vAmbient;
	Vector4								m_vDiffuse;
	Vector4								m_vSpecular;
	float								m_fRadius;
	float								m_fIntesity;
};

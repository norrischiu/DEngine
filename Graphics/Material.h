#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Math\simdmath.h"

typedef SIMDVector3 Vector4;

class Material
{
public:

	// Constructor
	Material(Vector4 amb, Vector4 diff, Vector4 spec, float shin)
		: m_vAmbient(amb)
		, m_vDiffuse(diff)
		, m_vSpecular(spec)
	{
		m_vSpecular.SetW(shin);
	}

private:

	Vector4						m_vAmbient;
	Vector4						m_vDiffuse;
	Vector4						m_vSpecular;  // w = shininess
};

#endif
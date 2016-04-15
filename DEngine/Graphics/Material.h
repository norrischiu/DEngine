// Material.h: class to hold mesh material at cpu side
#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Math\simdmath.h"
#include "Render\RenderTechnique.h"

namespace DE
{

class Material
{
public:

	// Empty constructor
	Material()
		: m_hRenderTechnique(sizeof(RenderTechnique))
	{
		new (m_hRenderTechnique) RenderTechnique();
	};

	// Overload constructor
	Material(Vector4 emi, Vector4 diff, Vector4 spec, float shin)
		: m_vEmissive(emi)
		, m_vDiffuse(diff)
		, m_vSpecular(spec)
		, m_fShininess(shin)
		, m_TexFlag(NULL)
		, m_hRenderTechnique(sizeof(RenderTechnique))
	{
		new (m_hRenderTechnique) RenderTechnique();
	}

	void ReadFromFile(const char* filename, int meshType);

	void UseDefault();

	void BindToRenderer();

	RenderTechnique* GetRenderTechnique()
	{
		return (RenderTechnique*) m_hRenderTechnique.Raw();
	}

	void Destruct()
	{
		m_hRenderTechnique.Free();
	}

	void AddPassToTechnique(RenderPass* pass)
	{
		((RenderTechnique*)m_hRenderTechnique.Raw())->AddPass(pass);
	}

	inline Vector4 GetSpecular()
	{
		return m_vSpecular;
	}

	inline float GetShininess()
	{
		return m_fShininess;
	}

private:

	Vector4						m_vEmissive;
	Vector4						m_vDiffuse;
	Vector4						m_vSpecular;
	float						m_fShininess;

	// Texture bitfield
	enum
	{
		DIFFUSE = 0x01,
		NORMAL = 0x02,
		SPECULAR = 0x04,
		GLOW = 0x08,
		DISPLACEMENT = 0x10
	};

	char						m_TexFlag;

	Handle						m_hRenderTechnique;
};

};
#endif
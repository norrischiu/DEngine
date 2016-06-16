// Material.h: class to hold mesh material at cpu side
#ifndef MATERIAL_H_
#define MATERIAL_H_

// Engine include
#include "Math\simdmath.h"
#include "Render\RenderTechnique.h"

namespace DE
{

/*
*	CLASS: Material
*	Material contains the attribute of a material under the
*	phong illumination model, namely emissive, diffuse,
*	specular and shininess
*/
class Material
{
public:

	/********************************************************************************
	*	--- Constructor:
	*	Material()
	*	This constructor will allocate memory for the default render technique
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	Material()
		: m_hRenderTechnique(sizeof(RenderTechnique))
	{
		new (m_hRenderTechnique) RenderTechnique();
	};

	/********************************************************************************
	*	--- Constructor:
	*	Material(Vector4 , Vector4, Vector4, float)
	*	This constructor will set attribute value and allocate memory for render
	*	technique
	*
	*	--- Parameters:
	*	@ emi: emissive factor
	*	@ diff: diffuse factor
	*	@ spec: specular factor
	*	@ shin: shininess factor
	********************************************************************************/
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

	/********************************************************************************
	*	--- Function:
	*	ReadFromFile(const char*, int)
	*	This function will set the factors value according to a material definition
	*	file and the mesh type
	*
	*	--- Parameters:
	*	@ filename: the name of material definition file
	*	@ meshType: the mesh type which then controls the default render technique
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void ReadFromFile(const char* filename, int meshType);

	/********************************************************************************
	*	--- Function:
	*	UseDefault()
	*	This function will assign default value for the factors of this material,
	*	with diffuse all 1 and shininess 1
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void UseDefault();

	/********************************************************************************
	*	--- Function:
	*	BindToRenderer()
	*	This function will bind all the value of this material to the constant buffer
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void BindToRenderer();

	/********************************************************************************
	*	--- Function:
	*	GetRenderTechnique()
	*	This function will return the pointer to the render technique of this material
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ RenderTechnique*: the pointer to the render technique of this material
	********************************************************************************/
	RenderTechnique* GetRenderTechnique()
	{
		return (RenderTechnique*) m_hRenderTechnique.Raw();
	}

	/********************************************************************************
	*	--- Function:
	*	Destruct()
	*	This function will free the memory used by render technique
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Destruct()
	{
		m_hRenderTechnique.Free();
	}

	/********************************************************************************
	*	--- Function:
	*	AddPassToTechnique(RenderPass*)
	*	This function will add a render pass to the render technique
	*
	*	--- Parameters:
	*	@ pass: pointer to a render pass to be added
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void AddPassToTechnique(RenderPass* pass)
	{
		((RenderTechnique*)m_hRenderTechnique.Raw())->AddPass(pass);
	}

	/********************************************************************************
	*	--- Function:
	*	GetSpecular()
	*	This function will return the specular factor
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ Vector4: specular factor of rgb
	********************************************************************************/
	inline Vector4 GetSpecular()
	{
		return m_vSpecular;
	}

	/********************************************************************************
	*	--- Function:
	*	GetShininess()
	*	This function will return the shininess factor
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ float: shininess factor
	********************************************************************************/
	inline float GetShininess()
	{
		return m_fShininess;
	}

private:

	/*
	*	ENUM
	*	Bitfield of information that this material contains
	*/
	enum
	{
		DIFFUSE = 0x01,
		NORMAL = 0x02,
		SPECULAR = 0x04,
		GLOW = 0x08,
		DISPLACEMENT = 0x10
	};

	Vector4						m_vEmissive;	// emissive factor
	Vector4						m_vDiffuse;		// diffuse factor
	Vector4						m_vSpecular;	// specular factor
	float						m_fShininess;	// shininess factor
	char						m_TexFlag;		// factors contained in a material definition file
	Handle						m_hRenderTechnique;		// handle referring to a rendering technique
};

};
#endif
// Material.h: class to hold mesh material at cpu side
#ifndef MATERIAL_H_
#define MATERIAL_H_

// Engine include
#include "Math\simdmath.h"
#include "Render\RenderPass.h"

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
	*	Empty constructor
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	Material()
		: m_vTextures(0)
	{
	};

	/********************************************************************************
	*	--- Constructor:
	*	Material(Vector4 , Vector4, Vector4, float)
	*	This constructor will set attribute value
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
		, m_vTextures(0)
	{
	}

	/********************************************************************************
	*	--- Function:
	*	ReadFromFile(const char*, int)
	*	This function will set the factors value according to a material definition
	*	file and the mesh type
	*
	*	--- Parameters:
	*	@ filename: the name of material definition file
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void ReadFromFile(const char* filename);

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
	*	Destruct()
	*	Empty destructor
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Destruct()
	{
		//m_vTextures.Clear();
	}

	/********************************************************************************
	*	--- Function:
	*	BindToRenderer(Renderer*)
	*	This function will bind the texture srv to renderer
	*
	*	--- Parameters:
	*	@ Renderer*: pointer to renderer
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void BindToRenderer(Renderer* renderer);


	/********************************************************************************
	*	--- Function:
	*	SetTexture(Texture*, unsigned int)
	*	This function will set the texture at the given slot
	*
	*	--- Parameters:
	*	@ tex: a pointer to a Texture class
	*	@ slot: slot
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetTexture(Handle tex, unsigned int slot)
	{
		m_vTextures.Add(tex);
	}

	void SetTextureCount(unsigned int count)
	{
		m_vTextures.Resize(count);
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

	MyArray<Handle>							m_vTextures;	// texture array
	Vector4									m_vEmissive;	// emissive factor
	Vector4									m_vDiffuse;		// diffuse factor
	Vector4									m_vSpecular;	// specular factor
	float									m_fShininess;	// shininess factor
	char									m_TexFlag;		// factors contained in a material definition file
};

};
#endif
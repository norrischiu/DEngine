#ifndef PS_PER_MATERIAL_CBUFFER_H_
#define PS_PER_MATERIAL_CBUFFER_H_

#include "CBuffer.h"
#include "Math\simdmath.h"

class PSPerMaterialCBuffer : public CBuffer
{

public:

	struct MaterialGPU
	{
		Vector4			vEmissive;
		Vector4			vDiffuse;
		Vector4			vSpecular;
		float			fShininess;
	};

	struct PS_PER_MATERIAL_CBUFFER
	{
		MaterialGPU		material;
	};

	PSPerMaterialCBuffer();

	void Update()
	{
		CBuffer::Update(NULL, NULL, sizePS);
	}

	static const size_t						sizeVS;
	static const size_t						sizePS;
};

#endif // !PS_PER_MATERIAL_CBUFFER_H_

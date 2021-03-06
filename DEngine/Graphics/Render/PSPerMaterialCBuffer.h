#ifndef PS_PER_MATERIAL_CBUFFER_H_
#define PS_PER_MATERIAL_CBUFFER_H_

#include "CBuffer.h"
#include "Math\simdmath.h"

namespace DE
{

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
		CBuffer::Update(size);
	}

	static const size_t						size;
};

};
#endif // !PS_PER_MATERIAL_CBUFFER_H_

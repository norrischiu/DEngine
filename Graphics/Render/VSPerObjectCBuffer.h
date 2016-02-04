#ifndef VS_PER_OBJECT_CBUFFER_H_
#define VS_PER_OBJECT_CBUFFER_H_

#include "CBuffer.h"
#include "Math\simdmath.h"

class VSPerObjectCBuffer : public CBuffer
{

public:

	struct VS_PER_OBJECT_CBUFFER
	{
		Matrix4			WorldTransform;
		Matrix4			Transform;
	};

	VSPerObjectCBuffer();

	void Update()
	{
		CBuffer::Update(sizeVS, NULL);
	}

	static const size_t						sizeVS;
	static const size_t						sizePS;
};

#endif // !VS_PER_OBJECT_CBUFFER_H_

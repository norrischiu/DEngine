#ifndef VS_PER_OBJECT_CBUFFER_H_
#define VS_PER_OBJECT_CBUFFER_H_

#include "CBuffer.h"
#include "Math\simdmath.h"

namespace DE
{

class VSPerObjectCBuffer : public CBuffer
{

public:

	struct VS_PER_OBJECT_CBUFFER
	{
		Matrix4			WorldTransform;
		Matrix4			WVPTransform;
	};

	VSPerObjectCBuffer();

	void Update()
	{
		CBuffer::Update(size);
	}

	static const size_t						size;
};

};
#endif // !VS_PER_OBJECT_CBUFFER_H_

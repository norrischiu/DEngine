#ifndef CBUFFER_H_
#define CBUFFER_H_

#include <d3d11.h>

namespace DE
{

class CBuffer
{

public:

	enum type
	{
		VertexShader,
		GeometryShader,
		PixelShader,
		HullShader,
		DomainShader,
	};

	CBuffer(int type, size_t size);

	// Set constant buffer to GPU, override this if need behavior different from default
	virtual void BindToRenderer();

	void Update(size_t size);

	struct CPU_GPU_MEMORY
	{
		void*							_data;

		// Pointer to vertex shader constant buffer
		ID3D11Buffer*					_buffer;
	};

	CPU_GPU_MEMORY						m_Memory;

	int									m_iType;

	// Register ID in GPU
	int									m_iSlotID;
};

};
#endif // !CBUFFER_H_

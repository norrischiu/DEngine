#ifndef CBUFFER_H_
#define CBUFFER_H_

#include <d3d11.h>

class CBuffer
{

public:

	enum type
	{
		VS_ONLY,
		GS_ONLY,
		PS_ONLY,
		VS_PS,
		VS_GS_PS
	};

	CBuffer(int type, size_t sizeVS = NULL, size_t sizeGS = NULL, size_t sizePS = NULL);

	void BindToRenderer();

	virtual void Update(size_t sizeVS = NULL, size_t sizeGS = NULL, size_t sizePS = NULL);

	struct CPU_GPU_MEMORY
	{
		void*							m_data;

		// Pointer to vertex shader constant buffer
		ID3D11Buffer*					m_pConstantBuffer;
	};

	CPU_GPU_MEMORY						VS, GS, PS;

	int									m_type;

	// Register ID in GPU
	int									m_iSlotID;
};

#endif // !CBUFFER_H_

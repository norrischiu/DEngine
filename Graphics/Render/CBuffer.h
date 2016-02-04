#ifndef CBUFFER_H_
#define CBUFFER_H_

#include <d3d11.h>

class CBuffer
{

public:

	enum type
	{
		VS_ONLY,
		PS_ONLY,
		VS_PS
	};

	CBuffer(int type, size_t sizeVS = NULL, size_t sizePS = NULL);

	void BindToRenderer();

	virtual void Update(size_t sizeVS = NULL, size_t sizePS = NULL);

	struct CPU_GPU_MEMORY
	{
		void*							m_data;

		// Pointer to vertex shader constant buffer
		ID3D11Buffer*					m_pConstantBuffer;
	};

	CPU_GPU_MEMORY						VS, PS;

	int									m_type;

	// Register ID in GPU
	int									m_iSlotID;
};


/*
struct LightGPU
{
	Vector3		vPos;
	Vector3		vDir;
	Vector4		vColor;
	float		fIntensity;
	float		fInnerAngle;
	float		fOuterAngle;
	float		fRadius;
	int			iType;
};

struct PS_PER_OBJECT_CBUFFER_LIGHTING
{
	LightGPU	light;
	Matrix4		mClipToView; // to compute view space position from screen space
};
*/

#endif // !CBUFFER_H_

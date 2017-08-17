#ifndef CBUFFER_H_
#define CBUFFER_H_

// D3D include
#include <d3d11.h>
#include <d3d12.h>
#include "GlobalInclude.h"
#include "Graphics/D3DRenderer.h"

namespace DE
{

/*
*	CLASS: CBuffer
*	CBuffer is the base class for every GPU constant buffer interface
*	stored in CPU side. Every constant buffer in GPU must be referred
*	to a derived class of this class
*/
class CBuffer
{

public:

	/*
	*	ENUM: type
	*	The shader stage using this constant buffer
	*/
	enum type
	{
		VertexShader,
		GeometryShader,
		PixelShader,
		HullShader,
		DomainShader,
	};

	/*
	*	STRUCT: CPU_GPU_MEMORY
	*	This holds the pointers of CPU and GPU memory representation
	*/
	struct CPU_GPU_MEMORY
	{
		void*							_data;		// Pointer to CPU constant buffer memory
#ifdef D3D12
		void*							_buffer;	// Pointer to GPU constant buffer memory
#elif defined D3D11
		ID3D11Buffer*					_buffer;	// Pointer to GPU constant buffer memory
#endif
	};

	static void InitializeCBVUploadHeap(D3DRenderer* renderer);
	static void Reset()
	{
		CBV_UPLOAD_HEAP_CURR_OFFSET = 0;
	}

	/********************************************************************************
	*	--- Constructor:
	*	CBuffer(int, size_t);
	*	This constructor will construct an CBuffer and allocate memory for the content
	*
	*	--- Parameters:
	*	@ type: the shader stage using this constant buffer
	*	@ size: size of this constant buffer
	********************************************************************************/
	CBuffer(int type, size_t size);

	/********************************************************************************
	*	--- Virtual Function:
	*	void BindToRenderer()
	*	This function will set this constant buffer to GPU, override this function
	*	if need behavior different from default
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	virtual void BindToRenderer();

	virtual void BindToRendererWithOffset(int parameterIndex, int offset);

	/********************************************************************************
	*	--- Function:
	*	Update(size_t)
	*	This function will map the memory at CPU side to GPU side and copy the content,
	*	using map-discard
	*
	*	--- Parameters:
	*	@ size: size of this constant buffer
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Update(size_t size);

	static 	ID3D12Resource*				CBV_UPLOAD_HEAP;
	static  UINT						CBV_UPLOAD_HEAP_CURR_OFFSET;
	static  void*						CBV_UPLOAD_HEAP_BEGIN;
	
	CPU_GPU_MEMORY						m_Memory;		// the CPU and GPU data pointer
	int									m_iType;		// the shader stage of this constant buffer
	int									m_iSlotID;		// constant buffer slot ID in GPU
	int									m_iSize;
	UINT8*								m_pGPUAddress;
	D3D12_CPU_DESCRIPTOR_HANDLE			m_CPUHandle;
	UINT								m_iUploadHeapOffset;
};

};
#endif // !CBUFFER_H_

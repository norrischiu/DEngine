#pragma once

#include "Graphics\GPU\GPUDataType.h"
#include "Graphics\d3dx12.h"
#include <d3d12.h>

namespace DE
{

#define DEFAULT_ALIGNMENT 256

class GPUCircularAllocator
{
public:

	// This constructor will automatically map the resource
	GPUCircularAllocator(ID3D12Resource* gpuHeap, size_t heapSize)
	{
		m_pCachedResource = gpuHeap;
		m_iSize = heapSize;
		m_iOffset = 0;

		// cpu
		CD3DX12_RANGE readRange(0, 0);
		gpuHeap->Map(0, &readRange, &m_pCPUStart);

		// gpu
		m_pGPUStart = reinterpret_cast<void*>(gpuHeap->GetGPUVirtualAddress());
	}

	// This constructor will automatically unmap the resource
	~GPUCircularAllocator()
	{
		m_pCachedResource->Unmap(0, nullptr);
	}

	GPUHandle Allocate(size_t size)
	{
		GPUHandle handle;
		handle._cpuAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_pCPUStart) + m_iOffset);
		handle._gpuAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(m_pGPUStart) + m_iOffset);

		if (m_iOffset + ((size + (DEFAULT_ALIGNMENT - 1)) & ~(DEFAULT_ALIGNMENT - 1)) >= m_iSize)
		{
			m_iOffset = 0;
		}
		else
		{
			m_iOffset += (size + (DEFAULT_ALIGNMENT - 1)) & ~(DEFAULT_ALIGNMENT - 1);
		}

		return handle;
	}

	void Reset()
	{
		m_iOffset = 0;
	}

private:

	void*						m_pGPUStart; // start of the gpu heap
	void*						m_pCPUStart; // start of the cpu location
	ID3D12Resource*				m_pCachedResource; // cached pointer for unmapping
	UINT64						m_iOffset; // current offset from start
	size_t						m_iSize; // total size

};

}

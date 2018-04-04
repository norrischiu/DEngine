#include "CBuffer.h"
#include "Math\simdmath.h"
#include "Graphics\D3D12Renderer.h"

namespace DE
{

CBuffer::CBuffer(int type, size_t size)
: m_iType(type)
, m_iSize(size)
{
	m_Memory._data = malloc(size);
}

void CBuffer::BindToRenderer()
{
}

void CBuffer::BindToRendererWithOffset(int parameterIndex, int offset)
{
	D3D12Renderer* renderer = ((D3D12Renderer*)D3DRenderer::GetInstance());
	renderer->m_pCommandList->SetGraphicsRootConstantBufferView(parameterIndex, reinterpret_cast<D3D12_GPU_VIRTUAL_ADDRESS>(m_Memory._gpuHandle._gpuAddress));
}

void CBuffer::Update(size_t size)
{
	D3D12Renderer* renderer = ((D3D12Renderer*)D3DRenderer::GetInstance());

	m_Memory._gpuHandle = renderer->m_pConstantBufferAllocator->Allocate(size);
	memcpy(m_Memory._gpuHandle._cpuAddress, m_Memory._data, size);
}

};
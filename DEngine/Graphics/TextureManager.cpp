#include "TextureManager.h"
#include "DDSTextureLoader12.h"
#include "D3D12Renderer.h"

#include <vector>

namespace DE
{

TextureManager* TextureManager::m_pInstance;

void* TextureManager::GetTexture(const char* filename)
{
	std::string name(filename);
	name = "../Assets/" + name;
	if (!m_mapTexture.Contain(name.c_str()))
	{
		LoadTexture(name.c_str());
	}
	return m_mapTexture[name.c_str()];
}

void TextureManager::LoadTexture(const char* filename)
{
	HRESULT hr;
	wchar_t* pName = new wchar_t[strlen(filename) + 1]; //TODO
	mbstowcs(pName, filename, strlen(filename) + 1);

	D3D12Renderer* renderer = (D3D12Renderer*)D3DRenderer::GetInstance();
	ID3D12Resource* texBuffer;
	std::unique_ptr<uint8_t[]> pTexResourceView;
	std::vector<D3D12_SUBRESOURCE_DATA> subresources; // mips map
	UINT64 SRVCPUAddress;

	hr = DirectX::LoadDDSTextureFromFile(((D3D12Renderer*)D3DRenderer::GetInstance())->m_pDevice, pName, &texBuffer, pTexResourceView, subresources);
	assert(hr == S_OK && "texture file error");

	UINT64 textureUploadBufferSize;
	ID3D12Resource* textureBufferUploadHeap;
	D3D12_RESOURCE_DESC textureDesc = texBuffer->GetDesc();

	renderer->m_pDevice->GetCopyableFootprints(&textureDesc, 0, subresources.size(), 0, m_SubresourceFootprintHolder, m_RowCountHolder, m_RowSizeHolder, &textureUploadBufferSize);
	
	hr = renderer->m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize), // resource description for a buffer (storing the image data in this heap just to copy to the default heap)
		D3D12_RESOURCE_STATE_GENERIC_READ, // We will copy the contents from this heap to the default heap
		nullptr,
		IID_PPV_ARGS(&textureBufferUploadHeap));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	renderer->m_pDevice->CreateShaderResourceView(texBuffer, &srvDesc, renderer->m_CbvSrvUavHeapHandle);
	SRVCPUAddress = renderer->m_CbvSrvUavHeapHandle.ptr;
	renderer->m_CbvSrvUavHeapHandle.Offset(1, renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	
	uintptr_t uploadAddress;
	textureBufferUploadHeap->Map(0, nullptr, reinterpret_cast<void**>(&uploadAddress));

	// memcpy
	for (uint32_t i = 0; i < subresources.size(); ++i)
	{
		D3D12_MEMCPY_DEST dest_data;
		dest_data.pData = (void*) (uploadAddress + m_SubresourceFootprintHolder[i].Offset);
		dest_data.RowPitch = m_SubresourceFootprintHolder[i].Footprint.RowPitch;
		dest_data.SlicePitch = m_SubresourceFootprintHolder[i].Footprint.RowPitch * m_RowCountHolder[i];
		for (UINT z = 0; z < m_SubresourceFootprintHolder[i].Footprint.Depth; ++z)
		{
			uint8_t const * src_slice = (uint8_t*) subresources[i].pData + subresources[i].SlicePitch * z;
			uint8_t* dest_slice = reinterpret_cast<uint8_t*>(dest_data.pData) + dest_data.SlicePitch * z;
			for (UINT y = 0; y < m_RowCountHolder[i]; ++y)
			{
				memcpy(dest_slice + dest_data.RowPitch * y, src_slice + subresources[i].RowPitch * y, m_RowSizeHolder[i]);
			}
		}
	}

	// add copy command to command list
	for (int i = 0; i < subresources.size(); ++i)
	{
		D3D12_TEXTURE_COPY_LOCATION dst = {};
		dst.pResource = texBuffer;
		dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dst.SubresourceIndex = i;
		D3D12_TEXTURE_COPY_LOCATION src = {};
		src.pResource = textureBufferUploadHeap;
		src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		src.PlacedFootprint = m_SubresourceFootprintHolder[i];
		renderer->m_pCommandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
	}

	renderer->m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

//	textureBufferUploadHeap->Unmap(0, nullptr);
	hr = renderer->m_pCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { renderer->m_pCommandList };
	renderer->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	hr = renderer->m_pCommandQueue->Signal(renderer->m_pFence[renderer->m_iCurrFrameIndex], renderer->m_iFenceValue[renderer->m_iCurrFrameIndex]);
	renderer->ResetCommandAllocatorAndList();
	
	m_mapTexture.Add(filename, (void*) SRVCPUAddress);
}

};
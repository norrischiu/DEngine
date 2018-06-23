#include "Texture.h"
#include "Graphics\D3D12Renderer.h"
#include "State.h"
#include "Graphics\TextureManager.h"
#include "GlobalInclude.h"
#include <algorithm>

namespace DE
{

Texture::Texture(int type, int sampleCount, const char * filename, int mipLevel)
	: m_type(type)
{
	Renderer* renderer = Renderer::GetInstance();
	HRESULT hr;

	// Early return: creating a srv from file
	if (type & SHADER_RESOURCES && filename != nullptr)
	{
		m_pSRV.ptr = (uintptr_t) TextureManager::GetInstance()->GetTexture(filename);
		m_iSampler = State::LINEAR_MIPMAP_MAX_LOD_SS;
		return;
	}

	if (type & RENDER_TARGET)
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTVDesc;
		RTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		RTVDesc.Texture2D.MipSlice = 0;
		RTVDesc.Texture2D.PlaneSlice = 0;

		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.Width = WINDOW_WIDTH;
		textureDesc.Height = WINDOW_HEIGHT;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Alignment = 0;
		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		clearValue.Color[0] = 0.0f;
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 0.0f;

		CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
		renderer->m_pDevice->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
			&clearValue,
			IID_PPV_ARGS(&m_pTexture));
		
		renderer->m_pDevice->CreateRenderTargetView(m_pTexture, &RTVDesc, renderer->m_RTVHeapHandle);
		m_pRTV = renderer->m_RTVHeapHandle;
		renderer->m_RTVHeapHandle.Offset(renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		m_Format = textureDesc.Format;
	}
	if (type & DEPTH_STENCIL)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		textureDesc.Width = WINDOW_WIDTH;
		textureDesc.Height = WINDOW_HEIGHT;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Alignment = 0;

		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
		hr = renderer->m_pDevice->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&m_pTexture));
		assert(hr == S_OK);

		renderer->m_pDevice->CreateDepthStencilView(m_pTexture, &depthStencilViewDesc, renderer->m_DSVHeapHandle);
		m_pDSV = renderer->m_DSVHeapHandle;
		renderer->m_DSVHeapHandle.Offset(renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV));
		m_Format = textureDesc.Format;
	}
	if (type & SHADER_RESOURCES)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.Format = (type & DEPTH_STENCIL) ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32G32B32A32_FLOAT;
		SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.Texture2D.MipLevels = mipLevel;
		SRVDesc.Texture2D.PlaneSlice = 0;

		if (m_pTexture == nullptr)
		{
			D3D12_RESOURCE_DESC textureDesc = {};
			textureDesc.MipLevels = 1;
			textureDesc.Format = SRVDesc.Format;
			textureDesc.Width = WINDOW_WIDTH;
			textureDesc.Height = WINDOW_HEIGHT;
			textureDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
			textureDesc.DepthOrArraySize = 1;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			textureDesc.Alignment = 0;

			CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
			hr = renderer->m_pDevice->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&textureDesc,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				nullptr,
				IID_PPV_ARGS(&m_pTexture));
			assert(hr == S_OK);
		}
		renderer->m_pDevice->CreateShaderResourceView(m_pTexture, &SRVDesc, renderer->m_CbvSrvUavHeapHandle);
		m_pSRV = renderer->m_CbvSrvUavHeapHandle;
		renderer->m_CbvSrvUavHeapHandle.Offset(renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		m_Format = SRVDesc.Format;
	}

	m_iSampler = State::NULL_STATE;
}

Texture::Texture(int type, ID3D12Resource* texSrc)
{
	Renderer* renderer = Renderer::GetInstance();
	HRESULT hr;
	if (type & RENDER_TARGET)
	{
		// backbuffer
		m_pTexture = texSrc;
		renderer->m_pDevice->CreateRenderTargetView(texSrc, nullptr, renderer->m_RTVHeapHandle);
		m_pRTV = renderer->m_RTVHeapHandle;
		renderer->m_RTVHeapHandle.Offset(renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		m_Format = DXGI_FORMAT_R8G8B8A8_UNORM; // TODO: verify this assumption
	}
	if (type & DEPTH_STENCIL_READ_ONLY)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDesc.Width = WINDOW_WIDTH;
		textureDesc.Height = WINDOW_HEIGHT;
		textureDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Alignment = 0;

		CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
		hr = renderer->m_pDevice->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_DEPTH_READ,
			nullptr,
			IID_PPV_ARGS(&m_pTexture));
		assert(hr == S_OK);

		renderer->m_pDevice->CreateDepthStencilView(m_pTexture, &depthStencilViewDesc, renderer->m_DSVHeapHandle);
		m_pDSV = renderer->m_DSVHeapHandle;
		renderer->m_DSVHeapHandle.Offset(renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV));
		m_Format = textureDesc.Format;
	}
	if (type & SHADER_RESOURCES)
	{
		renderer->m_pDevice->CreateShaderResourceView(texSrc, nullptr, renderer->m_CbvSrvUavHeapHandle);
		m_pSRV = renderer->m_CbvSrvUavHeapHandle;
		renderer->m_CbvSrvUavHeapHandle.Offset(renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		m_Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // TODO: verify this assumption
	}
	m_pTexture = texSrc;
	m_iSampler = State::NULL_STATE;
}
};
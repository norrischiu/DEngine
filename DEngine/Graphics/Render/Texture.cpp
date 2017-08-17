#include "Texture.h"
#include "Graphics\D3DRenderer.h"
#include "Graphics\D3D11Renderer.h"
#include "Graphics\D3D12Renderer.h"
#include "Graphics\DDSTextureLoader12.h"
#include <vector>
#include "State.h"
#include "Graphics\TextureManager.h"
#include "GlobalInclude.h"
#include <algorithm>

namespace DE
{

Texture::Texture(int type, int sampleCount, const char * filename, int mipLevel)
	: m_type(type)
{
	D3D12Renderer* renderer = (D3D12Renderer*)D3DRenderer::GetInstance();
	HRESULT hr;

	// Early return: creating a srv from file
	if (type & SHADER_RESOURCES && filename != nullptr)
	{
		
#ifdef D3D12
		ID3D12Resource* texBuffer;
		std::unique_ptr<uint8_t[]> pTexResourceView;
		std::vector<D3D12_SUBRESOURCE_DATA> subresources; // mips count
		std::string name(filename);
		name = "../Assets/" + name; // TODO: remove string operation
		wchar_t* pName = new wchar_t[strlen(name.c_str()) + 1]; //TODO
		mbstowcs(pName, name.c_str(), strlen(name.c_str()) + 1);
		hr = DirectX::LoadDDSTextureFromFile(((D3D12Renderer*)D3DRenderer::GetInstance())->m_pDevice, pName, &texBuffer, pTexResourceView, subresources);
		assert(hr == S_OK && "texture file error");
		//texBuffer = (ID3D12Resource*) TextureManager::GetInstance()->GetTexture(filename);

		// Upload heap
		UINT64 textureUploadBufferSize;
		ID3D12Resource* textureBufferUploadHeap;
		D3D12_RESOURCE_DESC textureDesc = texBuffer->GetDesc();
		renderer->m_pDevice->GetCopyableFootprints(&textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);
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
		m_pSRV = renderer->m_CbvSrvUavHeapHandle;
		renderer->m_CbvSrvUavHeapHandle.Offset(1, renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

		UpdateSubresources(renderer->m_pCommandList, texBuffer, textureBufferUploadHeap, 0, 0, 1, &subresources[0]);
		renderer->m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		m_iSampler = State::LINEAR_MIPMAP_MAX_LOD_SS;
#elif defined D3D11
		m_pSRV = (ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(filename);
		m_pSampler = (ID3D11SamplerState*)State::GetState(State::LINEAR_MIPMAP_MAX_LOD_SS);
#endif
		return;
	}

#ifdef D3D11
	// Setup the texture description
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = WINDOW_WIDTH;
	textureDesc.Height = WINDOW_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = sampleCount;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (type & SHADER_RESOURCES)
	{
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	if (type & RENDER_TARGET)
	{
		textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}
	if ((type & DEPTH_STENCIL) || (type & DEPTH_STENCIL_READ_ONLY))
	{
		textureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	}
#endif

	if (type & RENDER_TARGET)
	{
#ifdef D3D12
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
		((D3D12Renderer*)D3DRenderer::GetInstance())->m_pDevice->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
			&clearValue,
			IID_PPV_ARGS(&m_pTexture));
		
		renderer->m_pDevice->CreateRenderTargetView(m_pTexture, &RTVDesc, renderer->m_RTVHeapHandle);
		m_pRTV = renderer->m_RTVHeapHandle;
		renderer->m_RTVHeapHandle.Offset(renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
#elif defined D3D11
		// Setup the render target texture description.
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		// Setup the description of the render target view.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = (sampleCount > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D);
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create render target view
		hr = ((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device->CreateTexture2D(&textureDesc, 0, &m_pTexture);
		assert(hr == S_OK);
		hr = ((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device->CreateRenderTargetView(m_pTexture, &renderTargetViewDesc, &m_pRTV);
		assert(hr == S_OK);
#endif
		m_Format = textureDesc.Format;
	}
	if (type & DEPTH_STENCIL)
	{
#ifdef D3D12
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		hr = ((D3D12Renderer*)D3DRenderer::GetInstance())->m_pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pDSV));
		assert(hr == S_OK);

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
		hr = ((D3D12Renderer*)D3DRenderer::GetInstance())->m_pDevice->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&m_pTexture));
		assert(hr == S_OK);
		((D3D12Renderer*)D3DRenderer::GetInstance())->m_pDevice->CreateDepthStencilView(m_pTexture, &depthStencilViewDesc, m_pDSV->GetCPUDescriptorHandleForHeapStart());
#elif defined D3D11
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;

		hr = ((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device->CreateTexture2D(&textureDesc, 0, &m_pTexture);
		assert(hr == S_OK);
		hr = ((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device->CreateDepthStencilView(m_pTexture, &depthStencilViewDesc, &m_pDSV);
		assert(hr == S_OK);
#endif
		m_Format = textureDesc.Format;
	}
	if (type & SHADER_RESOURCES)
	{
#ifdef D3D12
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
#elif defined D3D11
		// Setup the description of the shader resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = (sampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D);
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = mipLevel;

		if (type & DEPTH_STENCIL)
		{
			shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			hr = ((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device->CreateShaderResourceView(m_pTexture, &shaderResourceViewDesc, &m_pSRV);
			assert(hr == S_OK);
		}
		else
		{
			hr = ((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device->CreateShaderResourceView(m_pTexture, &shaderResourceViewDesc, &m_pSRV);
			assert(hr == S_OK);
		}
#endif
		m_Format = SRVDesc.Format;
	}

#ifdef D3D12
	m_iSampler = State::NULL_STATE;
#elif defined D3D11
	m_pSampler = (ID3D11SamplerState*)State::GetState(State::NULL_STATE);
#endif
}

#ifdef D3D12
Texture::Texture(int type, ID3D12Resource* texSrc)
{
	D3D12Renderer* renderer = (D3D12Renderer*) D3DRenderer::GetInstance();
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
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		hr = renderer->m_pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pDSV));

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

		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = textureDesc.Format;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
		hr = renderer->m_pDevice->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_DEPTH_READ,
			&clearValue,
			IID_PPV_ARGS(&m_pTexture));
		assert(hr == S_OK);
		renderer->m_pDevice->CreateDepthStencilView(m_pTexture, &depthStencilViewDesc, m_pDSV->GetCPUDescriptorHandleForHeapStart());
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
#elif defined D3D11
Texture::Texture(int type, ID3D11Texture2D* texSrc)
{
	HRESULT hr;
	if (type & RENDER_TARGET)
	{
		hr = ((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device->CreateRenderTargetView(texSrc, NULL, &m_pRTV);
		assert(hr == S_OK);
	}
	if (type & SHADER_RESOURCES)
	{
		hr = ((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device->CreateShaderResourceView(texSrc, NULL, &m_pSRV);
		assert(hr == S_OK);
	}
	if (type & DEPTH_STENCIL_READ_ONLY)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;
		hr = ((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device->CreateDepthStencilView(texSrc, &depthStencilViewDesc, &m_pDSV);
		assert(hr == S_OK);
	}
	m_pTexture = texSrc;
	m_pSampler = (ID3D11SamplerState*)State::GetState(State::NULL_STATE);
}
#endif

};
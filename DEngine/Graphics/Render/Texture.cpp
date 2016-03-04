#include "Texture.h"
#include "Graphics\D3D11Renderer.h"
#include "State.h"
#include "Graphics\TextureManager.h"

namespace DE
{

Texture::Texture(int type, int sampleCount, const char * filename)
	: m_type(type)
{
	if (type & SHADER_RESOURCES && filename != nullptr)
	{
		m_pSRV = (ID3D11ShaderResourceView*)TextureManager::GetInstance()->GetTexture(filename);
		m_pSampler = (ID3D11SamplerState*)State::GetState(State::LINEAR_MIPMAP_MAX_LOD_SS);
		return;
	}

	HRESULT hr;
	// Setup the texture description
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = 1024;
	textureDesc.Height = 768;
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

	if (type & RENDER_TARGET)
	{
		// Setup the render target texture description.
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		// Setup the description of the render target view.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = (sampleCount > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D);
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create render target view
		hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateTexture2D(&textureDesc, 0, &m_pTexture);
		assert(hr == S_OK);
		hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateRenderTargetView(m_pTexture, &renderTargetViewDesc, &m_pRTV);
		assert(hr == S_OK);
	}
	if (type & DEPTH_STENCIL)
	{
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;

		hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateTexture2D(&textureDesc, 0, &m_pTexture);
		assert(hr == S_OK);
		hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateDepthStencilView(m_pTexture, &depthStencilViewDesc, &m_pDSV);
		assert(hr == S_OK);
	}
	if (type & SHADER_RESOURCES)
	{
		// Setup the description of the shader resource view.

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = (sampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D);
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		if (type & DEPTH_STENCIL)
		{
			shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateShaderResourceView(m_pTexture, &shaderResourceViewDesc, &m_pSRV);
			assert(hr == S_OK);
		}
		else
		{
			hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateShaderResourceView(m_pTexture, &shaderResourceViewDesc, &m_pSRV);
			assert(hr == S_OK);
		}
	}

	m_pSampler = (ID3D11SamplerState*)State::GetState(State::NULL_STATE);
}

Texture::Texture(int type, ID3D11Texture2D* texSrc)
{
	HRESULT hr;
	if (type & RENDER_TARGET)
	{
		hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateRenderTargetView(texSrc, NULL, &m_pRTV);
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
		hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateDepthStencilView(texSrc, &depthStencilViewDesc, &m_pDSV);
		assert(hr == S_OK);
	}
}

};
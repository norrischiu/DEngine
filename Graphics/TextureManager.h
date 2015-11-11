// TextureManager.h:

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <stdio.h>
#include <unordered_map>
#include <string>
#include "D3D11Renderer.h"
#include <d3d11shader.h>

enum eSamplerState
{
	NONE,
	LINEAR_MIPMAP_MAX_LOD
};


class TextureManager
{
public:

	TextureManager()
		: m_mapTexture()
	{
		D3D11_SAMPLER_DESC samplerDesc;

		// Set sampler description
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		D3D11Renderer::GetInstance()->m_pD3D11Device->CreateSamplerState(&samplerDesc, (ID3D11SamplerState**) &m_pSamplerState);

		D3D11_BLEND_DESC blendStateDesc;
		ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
		blendStateDesc.AlphaToCoverageEnable = FALSE;
		blendStateDesc.IndependentBlendEnable = FALSE;
		blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		D3D11Renderer::GetInstance()->m_pD3D11Device->CreateBlendState(&blendStateDesc, (ID3D11BlendState**) &m_pBlendState);
	};

	// Return the texture according to the name
	void* GetTexture(const char* filename);

	// Insert texture data into map
	void LoadTexture(const char* filename);

	// Return sampler state depends on the enum
	void* GetSamplerState(int samplerStateType);

	void* GetBlendState();

	// Return singleton instance
	static TextureManager* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new TextureManager();
		return m_pInstance;
	}

	static void DestructandCleanUp()
	{
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~TextureManager()
	{
		std::unordered_map<std::string, void*>::iterator itr;
		for (itr = m_mapTexture.begin(); itr != m_mapTexture.end(); ++itr)
		{
			ID3D11ShaderResourceView* pSRView = (ID3D11ShaderResourceView*)itr->second;
			pSRView->Release();
		}
		m_mapTexture.clear();

		if (m_pBlendState) 
			delete m_pBlendState;
		if (m_pSamplerState)
			delete m_pSamplerState;
	}

private:

	// Singleton instance
	static TextureManager*									m_pInstance;

	std::unordered_map<std::string, void*>					m_mapTexture;

	void*													m_pSamplerState;

	void*													m_pBlendState;
};

#endif // !TEXTUREMANAGER_H_
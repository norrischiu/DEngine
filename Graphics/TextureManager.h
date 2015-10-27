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
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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
		D3D11Renderer::getInstance()->m_pD3D11Device->CreateSamplerState(&samplerDesc, (ID3D11SamplerState**) &m_pSamplerState);
	};

	// Return the texture according to the name
	void* GetTexture(const char* filename);

	// Insert texture data into map
	void LoadTexture(const char* filename);

	// Return sampler state depends on the enum
	void* GetSamplerState(int samplerStateType);

	// Return singleton instance
	static TextureManager* getInstance()
	{
		if (!m_pInstance)
			m_pInstance = new TextureManager();
		return m_pInstance;
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
	}

private:

	// Singleton instance
	static TextureManager*									m_pInstance;

	FILE*													pFile;

	std::unordered_map<std::string, void*>					m_mapTexture;

	void*													m_pSamplerState;
};

#endif // !TEXTUREMANAGER_H_
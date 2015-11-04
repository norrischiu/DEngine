#include "TextureManager.h"
#include <DDSTextureLoader.h>

TextureManager* TextureManager::m_pInstance;

void* TextureManager::GetTexture(const char* filename)
{
	std::string name(filename);
	std::unordered_map<std::string, void*>::iterator result = m_mapTexture.find(name);
	if (result == m_mapTexture.end())
	{
		LoadTexture(filename);
		return m_mapTexture[filename];
	}
	else return (*result).second;
}

void TextureManager::LoadTexture(const char* filename)
{
	HRESULT hr;
	wchar_t* pName = new wchar_t[strlen(filename) + 1]; //TODO
	mbstowcs(pName, filename, strlen(filename) + 1);

	ID3D11Resource* pTexture;
	ID3D11ShaderResourceView* pTexResourceView;
	hr = DirectX::CreateDDSTextureFromFile(D3D11Renderer::GetInstance()->m_pD3D11Device, pName, &pTexture, &pTexResourceView);
	assert(hr == S_OK);

	m_mapTexture[filename] = (void*) pTexResourceView;
}

void* TextureManager::GetSamplerState(int samplerStateType)
{
	switch (samplerStateType)
	{
		case eSamplerState::NONE:
			return nullptr;
			break;
		case eSamplerState::LINEAR_MIPMAP_MAX_LOD:
			return m_pSamplerState;
	}
}

void* TextureManager::GetBlendState()
{
	return m_pBlendState;
}

#include "TextureManager.h"
#include "DDSLoader.h"
#include "D3D11Renderer.h"

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

	ID3D11Resource* pTexture;
	ID3D11ShaderResourceView* pTexResourceView;
	hr = CreateDDSTextureFromFile(D3D11Renderer::GetInstance()->m_pD3D11Device, pName, &pTexture, &pTexResourceView);
	assert(hr == S_OK);

	m_mapTexture.Add(filename, (void*)pTexResourceView);
}

};
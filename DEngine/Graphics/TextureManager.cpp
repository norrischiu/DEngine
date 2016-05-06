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
	std::unordered_map<std::string, void*>::iterator result = m_mapTexture.find(name);
	if (result == m_mapTexture.end())
	{
		LoadTexture(name.c_str());
		return m_mapTexture[name.c_str()];
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
	hr = CreateDDSTextureFromFile(D3D11Renderer::GetInstance()->m_pD3D11Device, pName, &pTexture, &pTexResourceView);
	assert(hr == S_OK);

	m_mapTexture[filename] = (void*)pTexResourceView;
}

};
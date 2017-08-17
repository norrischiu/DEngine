#include "TextureManager.h"
#include "DDSLoader.h"
//#include "DDSTextureLoader12.h"
#include "D3D11Renderer.h"
#include "D3D12Renderer.h"

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
#ifdef D3D12
	//ID3D12Resource* pTexture;
	//std::unique_ptr<uint8_t[]> pTexResourceView;
	//std::vector<D3D12_SUBRESOURCE_DATA> subresources; // mips count
	//hr = DirectX::LoadDDSTextureFromFile(((D3D12Renderer*)D3DRenderer::GetInstance())->m_pDevice, pName, &pTexture, pTexResourceView, subresources);
	//m_mapTexture.Add(filename, (void*)pTexture); // TODO: need both ID3D12Resource & raw data (ie subresources data)
#elif defined D3D11
	ID3D11Resource* pTexture;
	ID3D11ShaderResourceView* pTexResourceView;
	hr = CreateDDSTextureFromFile(((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Device, pName, &pTexture, &pTexResourceView);
	assert(hr == S_OK);
	m_mapTexture.Add(filename, (void*)pTexResourceView);
#endif
}

};
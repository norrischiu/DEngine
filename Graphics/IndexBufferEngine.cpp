#include "IndexBufferEngine.h"

unsigned int IndexBufferEngine::g_iCurrIndex = 0;

void IndexBufferEngine::CreateBuffer(const char * filename)
{
	unsigned int m_iNumTri, m_iNumIndics;
	void* pIndexData = nullptr;
	ID3D11Buffer* pIndexBuffer;
	HRESULT hr;

	// Read vertices
	pFile = fopen(filename, "r");
	char c[256];
	fscanf(pFile, "%s", &c); // name
	fscanf(pFile, "%s", &c); // num of vertex of prim (must be 3 for now)
	fscanf(pFile, "%i", &m_iNumTri);
	m_iNumIndics = m_iNumTri * 3;

	FillIndexData(m_iNumIndics, pIndexData);

	// Set index buffer description
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_iNumIndics;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Set index subresources data
	D3D11_SUBRESOURCE_DATA indexResourcesData;
	indexResourcesData.pSysMem = pIndexData;
	indexResourcesData.SysMemPitch = 0;
	indexResourcesData.SysMemSlicePitch = 0;

	// Create the index buffer
	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateBuffer(&indexBufferDesc, &indexResourcesData, &pIndexBuffer);
	assert(hr == S_OK);
	D3D11Renderer::getInstance()->m_pD3D11Context->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	g_iCurrIndex += m_iNumIndics;
}

void IndexBufferEngine::FillIndexData(unsigned int indicsNum, void *& pIndexData)
{
	pIndexData = new UINT[indicsNum]; // needs change memory allocation

	for (unsigned int i = 0; i < indicsNum; i++)
	{
		unsigned int x;
		fscanf(pFile, "%i", &x);
		((UINT*) pIndexData)[i] = x;
	}
	fclose(pFile);
}

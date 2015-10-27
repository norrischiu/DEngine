#include "IndexBufferEngine.h"

void* IndexBufferEngine::CreateBuffer(const char * filename, unsigned int& indicsNum)
{
	unsigned int iNumTri;
	void* pIndexData = nullptr;
	ID3D11Buffer* pIndexBuffer;
	HRESULT hr;

	// Read vertices
	pFile = fopen(filename, "r");
	char c[256];
	fscanf(pFile, "%s", &c); // name
	fscanf(pFile, "%s", &c); // num of vertex of prim (must be 3 for now)
	fscanf(pFile, "%i", &iNumTri);
	indicsNum = iNumTri * 3;

	FillIndexData(indicsNum, pIndexData);

	// Set index buffer description
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * indicsNum;
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

	return pIndexBuffer;
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

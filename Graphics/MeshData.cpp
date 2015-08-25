// MeshData.cpp

#include <MeshData.h>
#include <..\Math\simdmath.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

#pragma comment (lib, "D3DCompiler")

typedef SIMDVector3 Vector3;

// Vertex: Position
struct Vertex1P
{
	Vector3 m_pos;
};

// Vertex: Position, diffuse color
struct Vertex1P1D
{
	Vector3 m_pos;
	DirectX::XMFLOAT4 m_diffuse; // 16-byte offset
};

// Vertex: Position, normal, texture coordinate
struct Vertex1P1N1UV
{
	Vector3 m_pos;
	Vector3 m_norm; // 16-byte offset
	float m_UV[2]; // 32-byte offset
};

D3D11_INPUT_ELEMENT_DESC vertex1PInputDesc[]
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

D3D11_INPUT_ELEMENT_DESC vertex1P1DInputDesc[]
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

struct VS_CONSTANT_BUFFER
{
	Matrix4 CameraMat;
	Matrix4 ProjectionMat;
};

MeshData::MeshData(const char* filename)
	: m_pVertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
	//, m_pTexture(nullptr)
	, m_camera(Vector3(0, 3, -10), Vector3(1, 0, 0), Vector3(0, 1, 0))
{
	char* compileErrors;

	// Set vertex shader
	ID3DBlob* VS;
	ID3D11VertexShader* pVS;
	ID3DBlob* error;
	HRESULT hr = D3DCompileFromFile(L"Shaders/vertex1P1D.vs", NULL, NULL, "VS_Blue", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &VS, &error);
	if (error) compileErrors = (char*)(error->GetBufferPointer());
	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	D3D11Renderer::getInstance()->m_pD3D11Context->VSSetShader(pVS, 0, 0);

	// Initialize input layout
	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateInputLayout(vertex1P1DInputDesc, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &inputLayout);
	assert(hr == S_OK);
	D3D11Renderer::getInstance()->m_pD3D11Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11Renderer::getInstance()->m_pD3D11Context->IASetInputLayout(inputLayout);

	// Set pixel shader
	ID3DBlob* PS;
	ID3D11PixelShader* pPS;
	hr = D3DCompileFromFile(L"Shaders/diffuse.ps", NULL, NULL, "PS_Blue", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &PS, &error);
	if (error) compileErrors = (char*)(error->GetBufferPointer());
	D3D11Renderer::getInstance()->m_pD3D11Device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
	D3D11Renderer::getInstance()->m_pD3D11Context->PSSetShader(pPS, 0, 0);

	// Read vertices
	std::ifstream file;
	file.open("dragon_vertex.bufa");
	char c[256];
	file.getline(c, 255);
	file >> m_iNumVerts;
	Vertex1P1D* verticesTemp = new Vertex1P1D[m_iNumVerts]; // needs change memory allocation
	for (unsigned int i = 0; i < m_iNumVerts; i++)
	{
		float x, y, z;
		file >> x;
		file >> y;
		file >> z;
		verticesTemp[i] = { Vector3(x, y, z), DirectX::XMFLOAT4(1.0, 0.0, 0.0, 0.0) };
	}

	file.close();

	// Set vertex subresources data
	D3D11_SUBRESOURCE_DATA vertexResourcesData;
	vertexResourcesData.pSysMem = verticesTemp; // <---------------
	vertexResourcesData.SysMemPitch = 0;
	vertexResourcesData.SysMemSlicePitch = 0;

	// Set vertex buffer description
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex1P1D)* m_iNumVerts;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Create the vertex buffer
	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateBuffer(&vertexBufferDesc, &vertexResourcesData, &m_pVertexBuffer);
	assert(hr == S_OK);
	UINT stride = sizeof(Vertex1P1D);
	UINT vertexOffset = 0;
	D3D11Renderer::getInstance()->m_pD3D11Context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &vertexOffset);

	// Read indics
	file.open("dragon_index.bufa");
	unsigned int m_iNumTri;
	file.getline(c, 255); // version
	file.getline(c, 255); // num of vertex of prim (must be 3 for now)
	file >> m_iNumTri; // num of tri
	m_iNumIndics = m_iNumTri * 3;
	UINT* indicesTemp = new UINT[m_iNumIndics]; // needs change memory allocation
	for (unsigned int i = 0; i < m_iNumIndics; i++)
	{
		unsigned int x;
		file >> x;
		indicesTemp[i] = x;
	}

	file.close();

	// Set index buffer description
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_iNumIndics;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	
	// Set index subresources data
	D3D11_SUBRESOURCE_DATA indexResourcesData;
	indexResourcesData.pSysMem = indicesTemp;
	indexResourcesData.SysMemPitch = 0;
	indexResourcesData.SysMemSlicePitch = 0;

	// Create the index buffer
	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateBuffer(&indexBufferDesc, &indexResourcesData, &m_pIndexBuffer);
	assert(hr == S_OK);
	UINT indexOffset = 0;
	D3D11Renderer::getInstance()->m_pD3D11Context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, indexOffset);
	
	// Set constant buffer description
	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;

	// Set constant subresources data
	D3D11_SUBRESOURCE_DATA constResourcesData;

	VS_CONSTANT_BUFFER vsConstData;
	vsConstData.CameraMat.CreateLookAt(Vector3(0.0f, 3.0f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); // temp
	vsConstData.ProjectionMat.CreatePerspectiveFOV(0.785398163f, 1042.0f / 768.0f, 1.0f, 1000.0f);
	vsConstData.ProjectionMat.Multiply(vsConstData.CameraMat);
	Matrix4 rotation;
	rotation.CreateRotationY(0.785398163f); // 45 degree
	vsConstData.ProjectionMat.Multiply(rotation);

	constResourcesData.pSysMem = &vsConstData;
	constResourcesData.SysMemPitch = 0;
	constResourcesData.SysMemSlicePitch = 0;

	// Create the constant buffer
	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateBuffer(&constBufferDesc, &constResourcesData, &g_pConstantBuffer);
	assert(hr == S_OK);
	D3D11Renderer::getInstance()->m_pD3D11Context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	// Clear up
	if (VS)
		VS->Release();
	if (PS)
		PS->Release();
	if (error)
		error->Release();
	if (pVS)
		pVS->Release();
	if (pPS)
		pPS->Release();

}

void MeshData::Update()
{
	// Set constant buffer description
	m_camera.update();
	Matrix4 ProjectionMat;
	ProjectionMat.CreatePerspectiveFOV(0.785398163f, 1042.0f / 768.0f, 1.0f, 1000.0f);
	ProjectionMat.Multiply(m_camera.getViewMatrix());

	D3D11_MAPPED_SUBRESOURCE mappedResources;
	D3D11Renderer::getInstance()->m_pD3D11Context->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResources);
	VS_CONSTANT_BUFFER* pCBuffer = (VS_CONSTANT_BUFFER*)mappedResources.pData;
	pCBuffer->ProjectionMat = ProjectionMat;
	D3D11Renderer::getInstance()->m_pD3D11Context->Unmap(g_pConstantBuffer, 0);
}

void MeshData::Render()
{
	Update();
	D3D11Renderer::getInstance()->m_pD3D11Context->DrawIndexed(m_iNumIndics, 0, 0);
}
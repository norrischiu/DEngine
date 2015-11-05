// MeshData.h: a class contains all pointers to buffer of a mesh

#ifndef MESHDATA_H
#define MESHDATA_H

#include <d3d11.h>
#include "../Object/Camera.h"
#include "VertexFormat.h"
#include <vector>

enum eRenderType
{
	OUTLINE,
	STANDARD_MESH,
	STANDARD_MESH_WITH_SHADOW,
	V1P,
	V1P1UV,
	V1P1N,
	V1P1N1UV
};

class MeshData
{
public:

	// Overload default constructor
	MeshData(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndics, const Vector3& dimension, const eRenderType eRenderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture);

	MeshData(const char* filename, int renderType);

	~MeshData();

	void SetUpEnvironment(const eRenderType eRenderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture);

	void Transform(
		const float scalar = 1.0f,
		const Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f),
		const Vector3 translation = Vector3(0.0f, 0.0f, 0.0f)
	);

	void Update(Matrix4 transform);

	void Update();

	void Render(Matrix4 transform);

	void Render();

	inline int GetVertexNum()
	{
		return m_iNumVerts;
	}

	inline Vector3 GetVMin()
	{
		Vector3 vMin = Vector3(
			0.0f + m_dimension.GetX() / 2,
			0.0f - m_dimension.GetY() / 2,
			0.0f + m_dimension.GetZ() / 2
		);
		vMin.Transform(m_transformationMat);
		return vMin;
	}

	inline Vector3 GetVMax()
	{
		Vector3 vMax = Vector3(
			0.0f - m_dimension.GetX() / 2,
			0.0f + m_dimension.GetY() / 2,
			0.0f - m_dimension.GetZ() / 2
		);
		vMax.Transform(m_transformationMat);
		static wchar_t s[64];
		swprintf(s, 64, L"vMax: %f, %f, %f\n", vMax.GetX(), vMax.GetY(), vMax.GetZ());
		OutputDebugStringW(s);
		return vMax;
	}

	inline Vector3 GetCenter()
	{
		Vector3 center = Vector3(0.0f, 0.0f, 0.0f);
		center.TransformAsVector(m_transformationMat);
		return center;
	}

	inline Vector3 GetDimension()
	{
		return m_dimension;
	}

private:

	// Pointer to complied to vertex shader
	ID3D11VertexShader*						m_pVS;
	
	// Pointer to complied to pixel shader
	ID3D11PixelShader*						m_pPS;

	// Pointer to vertex buffer
	ID3D11Buffer*							m_pVertexBuffer;

	// Data size of a vertex
	unsigned int							m_iStride;

	// Offset in vertex buffer between first element and first to be used element
	unsigned int							m_iVertexOffset;

	// Pointer to index buffer
	ID3D11Buffer*							m_pIndexBuffer;

	// Pointer to constant buffer
	ID3D11Buffer*							g_pConstantBuffer;

	// Pointer to input layout supply to IA
	ID3D11InputLayout*						m_pInputLayout;

	// Pointer to resource view array pass to shader
	std::vector<ID3D11ShaderResourceView*>	m_pTexResourceView;

	// Pointer to sampler state
	ID3D11SamplerState*						m_pSamplerState;

	// Number of shader resource
	unsigned int							m_iTexResourceNum;

	// Pointer to blend state
	ID3D11BlendState*						m_pBlendState;

	// Primitive Topology
	unsigned int							m_iTopology;

	// Render Type
	eRenderType								m_renderType;

	// Number of vertices
	unsigned int							m_iNumVerts;

	// Number of indics
	unsigned int							m_iNumIndics;

	//Vertex offset
	unsigned int							m_vertexOffset;
 
	//Start Index Location
	unsigned int							m_iStartIndexLocation;

	Matrix4									m_transformationMat;

	Vector3									m_dimension;
};

#endif
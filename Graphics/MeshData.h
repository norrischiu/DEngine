// MeshData.h: a class contains all pointers to buffer of a mesh

#ifndef MESHDATA_H
#define MESHDATA_H

#include <d3d11.h>
#include <vector>
#include "Math\simdmath.h"
#include "Material.h"
#include "Physics\cdAABB.h"

enum eRenderType
{
	OUTLINE,
	STANDARD_MESH,
	STANDARD_MESH_WITH_SHADOW,
	STANDARD_MESH_WITH_BUMP,
	CUSTOM_SHADERS,
	V1P,
	V1P1UV
};

class MeshData
{
public:

	// Overload default constructor
	MeshData(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndics, const Vector3& dimension, const eRenderType eRenderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture);
	
	MeshData(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndics);

	MeshData(const char* filename, int renderType);

	MeshData() {};

	~MeshData();

	void SetUpEnvironment(const eRenderType eRenderType, const D3D_PRIMITIVE_TOPOLOGY typology, const char* texture);

	void Transform(
		const float scalar = 1.0f,
		const Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f),
		const Vector3 translation = Vector3(0.0f, 0.0f, 0.0f)
	);

	void Update();

	void Render();

	void RenderUsingPassAt(int index);

	void RenderUsingPass(RenderPass* pass);

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

	inline AABB GetBoundingBox()
	{
		return m_BoundingBox;
	}

	// Mesh material
	Material								m_Material;

private:

	// Pointer to vertex buffer
	ID3D11Buffer*							m_pVertexBuffer;

	// Data size of a vertex
	unsigned int							m_iStride;

	// Offset in vertex buffer between first element and first to be used element
	unsigned int							m_iVertexOffset;

	// Pointer to index buffer
	ID3D11Buffer*							m_pIndexBuffer;

	// Render Type
	eRenderType								m_renderType;

	// Number of vertices
	unsigned int							m_iNumVerts;

	// Number of indics
	unsigned int							m_iNumIndics;
 
	//Start Index Location
	unsigned int							m_iStartIndexLocation;

	Matrix4									m_transformationMat;

	Vector3									m_dimension;

	// Simple bounding box for camera frustum culling
	AABB									m_BoundingBox;

	// Mesh material
	//Material								m_Material;
};

#endif
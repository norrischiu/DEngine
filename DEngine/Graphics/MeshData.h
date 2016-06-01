// MeshData.h: a class contains all pointers to buffer of a mesh

#ifndef MESHDATA_H
#define MESHDATA_H

#include <d3d11.h>
#include "Math\simdmath.h"
#include "Material.h"
#include "Physics\cdAABB.h"

namespace DE
{

enum eMeshType
{
	OUTLINE,
	STANDARD_MESH,
	SKELETAL_MESH,
	V1P,
	V1P1UV
};

class MeshData
{
public:

	// Overload default constructor
	MeshData(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndics, unsigned int stride = 16, bool streamOut = false);

	MeshData(const char* filename, int meshType);

	MeshData() {};

	~MeshData();

	void Render();

	void RenderUsingPass(RenderPass* pass);

	inline int GetVertexNum()
	{
		return m_iNumVerts;
	}

	inline AABB GetBoundingBox()
	{
		return m_BoundingBox;
	}

	inline void SetBoundingBox(AABB aabb)
	{
		m_BoundingBox = aabb;
	}

	inline ID3D11Buffer* GetVertexBuffer()
	{
		return m_pVertexBuffer;
	}

	void Destruct()
	{
		m_Material.Destruct();
		m_pVertexBuffer->Release();
		m_pIndexBuffer->Release();
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
	eMeshType								m_renderType;

	// Number of vertices
	unsigned int							m_iNumVerts;

	// Number of indics
	unsigned int							m_iNumIndics;

	//Start Index Location
	unsigned int							m_iStartIndexLocation;

	// Flag whether it is stream out result
	bool									m_bStreamOut;

	// Simple bounding box for camera frustum culling
	AABB									m_BoundingBox;
};

};
#endif
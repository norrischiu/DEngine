// MeshData.h: a class contains all pointers to buffer of a mesh
#ifndef MESHDATA_H
#define MESHDATA_H

// D3D12 include
#include <d3d12.h>

// Engine include
#include "Graphics\D3D12Renderer.h"
#include "Math\simdmath.h"
#include "Material.h"
#include "Physics\cdAABB.h"
#include "GlobalInclude.h"

namespace DE
{

/*
*	ENUM: eMeshType
*	The type of mesh
*/
enum eMeshType
{
	OUTLINE,
	STANDARD_MESH,
	SKELETAL_MESH,
};

/*
*	CLASS: MeshData
*	MeshData contains the vertex buffer and index buffer
*	of a mesh, and the material to render this mesh
*/
class MeshData
{
public:

	/********************************************************************************
	*	--- Constructor:
	*	MeshData(void*, const int, unsigned int*, const int, unsigned int, bool)
	*	This constructor will construct a mesh data with given raw vertices and indices
	*	data in array form
	*
	*	--- Parameters:
	*	@ pVertexData: the vertices array in any format, refer to vertexformat.h for
	*	example
	*	@ iNumVerts: the number of vertex in the above array
	*	@ pIndexData: the indices array
	*	@ iNumIndics: the number of index in the above array
	*	@ stride: the size of a vertex in the given array
	*	@ streamOut: True if this mesh will be used for stream out; False if this mesh
	*	will NOT be used for stream out
	********************************************************************************/
	MeshData(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndics, unsigned int stride = 16, bool streamOut = false);

	/********************************************************************************
	*	--- Constructor:
	*	MeshData(const char*, int)
	*	This constructor will construct a mesh data from a file name, which then
	*	read from all the definition files (for example texture coordinate, skin
	*	weight) of the same name depending on the mesh type
	*
	*	--- Parameters:
	*	@ filename: the prefix file name of all the mesh definition files
	*	@ meshType: type of mesh as in eMeshType
	********************************************************************************/
	MeshData(const char* filename, int meshType);

	/********************************************************************************
	*	--- Constructor:
	*	MeshData()
	*	This constructor will do nothing
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	MeshData() {};

	/********************************************************************************
	*	--- Destructor:
	*	~MeshData()
	*	Empty destructor
	********************************************************************************/
	~MeshData();
	
	/********************************************************************************
	*	--- Function:
	*	Render()
	*	This function will draw the mesh by binding the vertex and index buffer to
	*	the GPU and calling the appropriate draw call
	*
	*	--- Parameters:
	*	@ Renderer*: pointer to renderer
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Render(Renderer* renderer);

	/********************************************************************************
	*	--- Function:
	*	GetVertexNum()
	*	This function will return the number of vertex of this mesh
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ int: number of vertex
	********************************************************************************/
	inline int GetVertexNum()
	{
		return m_iNumVerts;
	}

	/********************************************************************************
	*	--- Function:
	*	GetVertexNum()
	*	This function will return the default bounding box created from the max and
	*	min given vertices
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ AABB: the bounding box of this mesh
	********************************************************************************/
	inline AABB GetBoundingBox()
	{
		return m_BoundingBox;
	}

	/********************************************************************************
	*	--- Function:
	*	SetBoundingBox(AABB)
	*	This function will set the bounding box of this mesh, can be used to override
	*	the default created bounding box
	*
	*	--- Parameters:
	*	@ aabb: a bounding box
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	inline void SetBoundingBox(AABB aabb)
	{
		m_BoundingBox = aabb;
	}

	/********************************************************************************
	*	--- Function:
	*	Destruct()
	*	Empty destructor
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Destruct()
	{
	}

private:

	D3D12_VERTEX_BUFFER_VIEW				m_VBV;
	D3D12_INDEX_BUFFER_VIEW					m_IBV;

	unsigned int							m_iStride;		// Data size of a vertex
	unsigned int							m_iVertexOffset;	// Offset in vertex buffer between first element and first to be used element
	eMeshType								m_renderType;		// Render Type
	unsigned int							m_iNumVerts;		// Number of vertices
	unsigned int							m_iNumIndics;		// Number of indics
	unsigned int							m_iStartIndexLocation;	// Start Index Location
	bool									m_bStreamOut;		// Flag whether it is stream out result
	AABB									m_BoundingBox;		// Simple bounding box for camera frustum culling

};

};
#endif
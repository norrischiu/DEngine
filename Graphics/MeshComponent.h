// MeshComponent.h: the class for a mesh instance which contains a mesh data

#ifndef MeshComponent_H
#define MeshComponent_H

#include "MeshData.h"
#include "VertexFormat.h"

typedef SIMDVector3 Vector3;
typedef SIMDQuaternion Quaternion;

class MeshComponent
{
public:
	MeshComponent(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndices, const Vector3& dimension, const eRenderType eRenderType, const D3D11_PRIMITIVE_TOPOLOGY typology, const char* texture = NULL);
	MeshComponent(const char* filename);
	~MeshComponent() {};

	MeshData* GetMeshData();

	void Draw();

	// temp
	MeshData*								m_pMeshData;

private:
	// Contains all buffer and shaders data
	//MeshData*								m_pMeshData;

	// Quaternion
	Quaternion								m_quat;

	// Position vector
	Vector3									m_pos;
	
};

#endif
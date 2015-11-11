// MeshComponent.h: the class for a mesh instance which contains a mesh data

#ifndef MeshComponent_H
#define MeshComponent_H

#include "MeshData.h"
#include "VertexFormat.h"
#include "Scene\MeshSceneNode.h"

typedef SIMDVector3 Vector3;
typedef SIMDQuaternion Quaternion;

class MeshComponent
{
public:
	MeshComponent(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndices, const Vector3& dimension, const eRenderType eRenderType, const D3D11_PRIMITIVE_TOPOLOGY typology, const char* texture = NULL);
	MeshComponent(const char* filename);
	MeshComponent() {};

	MeshData* GetMeshData();

	void Draw();

	// temp
	MeshData*								m_pMeshData;

	// temp
	Matrix4*								m_pTransform;

private:
	// Contains all buffer and shaders data
	//MeshData*								m_pMeshData;

	// Scene node
	MeshSceneNode*							m_SceneNode;

	// Transform reference
	//Matrix4*								m_pTransform;
	
};

#endif
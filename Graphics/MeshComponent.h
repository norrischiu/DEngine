// MeshComponent.h: the class for a mesh instance which contains a mesh data

#ifndef MeshComponent_H
#define MeshComponent_H

#include "Object\Component.h"
#include "VertexFormat.h"
#include "Graphics\MeshData.h"

class MeshComponent : public Component
{

public:

	static const int ComponentID = ComponentID::MESH;

	MeshComponent(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndices, const Vector3& dimension, const eRenderType eRenderType, const D3D11_PRIMITIVE_TOPOLOGY typology, const char* texture = NULL);
	MeshComponent(const char* meshName);

	// Inherited via Component
	void Update(float deltaTime) override
	{
	};

	MeshData* GetMeshData();

	void Draw();

	// temp
	MeshData*								m_pMeshData;

	// temp
	Matrix4*								m_pTransform;

private:

	// Contains all buffer and shaders data
	//MeshData*								m_pMeshData;
	
};

#endif
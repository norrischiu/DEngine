// MeshComponent.h: the class for a mesh instance which contains a mesh data
#ifndef MeshComponent_H_
#define MeshComponent_H_

#include "Object\Component.h"
#include "Graphics\MeshData.h"

class MeshComponent : public Component
{

public:

	static const int ComponentID = ComponentID::MESH;

	MeshComponent(void* pVertexData, const int iNumVerts, unsigned int* pIndexData, const int iNumIndices, const Vector3& dimension, const eRenderType eRenderType, const D3D11_PRIMITIVE_TOPOLOGY typology, const char* texture = NULL);
	MeshComponent(const char* meshName);

	// Inherited via Component
	void SetOwner(GameObject* ptr) override;
	void Update(float deltaTime) override
	{
	};

	MeshData* GetMeshData();

	void Draw();

	// temp
	MeshData*								m_pMeshData;

	// temp
	Matrix4*								m_pTransform;

	// Visibility flag
	bool									m_bVisible;

private:

	// Contains all buffer and shaders data
	//MeshData*								m_pMeshData;

};

#endif // !MeshComponent_H_
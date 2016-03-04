// MeshComponent.h: the class for a mesh instance which contains a mesh data
#ifndef MeshComponent_H_
#define MeshComponent_H_

#include "Object\Component.h"
#include "Graphics\MeshData.h"

namespace DE
{

class MeshComponent : public Component
{

public:

	static const int ComponentID = ComponentID::MESH;
	
	// Overload constructor: create with given mesh data
	MeshComponent(MeshData* meshData);
	
	// Overload constructor: create with given mesh file name
	MeshComponent(const char* meshName, int type = eMeshType::STANDARD_MESH);

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

};

#endif // !MeshComponent_H_
// MeshComponent.h: one of the components that can be added to game object
#ifndef MeshComponent_H_
#define MeshComponent_H_

// Engine include
#include "Graphics\D3D12Renderer.h"
#include "Object\Component.h"
#include "Graphics\MeshData.h"

namespace DE
{

/*
*	CLASS: MeshComponent
*	MeshComponent is one kind of components that can be added
*	to a game object. This contains a mesh data
*/
class MeshComponent : public Component
{

public:

	static const int ComponentID = ComponentID::MESH;	// component ID
	
	/********************************************************************************
	*	--- Constructor:
	*	MeshComponent(MeshData*)
	*	This constructor will create a mesh component with a give mesh data, the
	*	component will NOT be added to the scene graph automatically
	*
	*	--- Parameters:
	*	@ meshData: pointer to a mesh data
	********************************************************************************/
	MeshComponent(MeshData* meshData);
	
	/********************************************************************************
	*	--- Constructor:
	*	MeshComponent(const char*, int)
	*	This constructor will create a mesh component by reading the definition files
	*	with the given name, and decide its render technique with given type
	*
	*	--- Parameters:
	*	@ meshName: the name of the mesh for looking up files in Assets folder
	*	@ type: mesh type as defined in eMeshType 
	********************************************************************************/
	MeshComponent(const char* meshName, int type = eMeshType::STANDARD_MESH);

	/********************************************************************************
	*	--- Destructor:
	*	~MeshComponent()
	*	This destructor will do nothing
	********************************************************************************/
	~MeshComponent() {};

	/********************************************************************************
	*	--- Function:
	*	Destruct()
	*	This function will destruct the mesh data
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Destruct()
	{
		m_pMeshData->Destruct();
		delete m_pMeshData;
	}


	/********************************************************************************
	*	--- Function:
	*	SetOwner(GameObject*)
	*	Inherited via Component
	*	This function will set the owner of this component and cache its transform
	*
	*	--- Parameters:
	*	@ ptr: pointer to a game object
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetOwner(GameObject* ptr) override;

	/********************************************************************************
	*	--- Function:
	*	Update(float)
	*	Inherited via Component
	*	This function will do nothing
	*
	*	--- Parameters:
	*	@ deltaTime: frame's delta time
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Update(float deltaTime) override
	{
	};


	/********************************************************************************
	*	--- Function:
	*	GetMeshData()
	*	This function will return the mesh data in this mesh component
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ MeshData*: a pointer to the mesh data
	********************************************************************************/
	MeshData* GetMeshData();

	/********************************************************************************
	*	--- Function:
	*	GetMaterial()
	*	This function will return the material in this mesh component
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ Material*: a pointer to the material
	********************************************************************************/
	Material* GetMaterial();

	/********************************************************************************
	*	--- Function:
	*	IsVisible()
	*	This function will return if this mesh component is visible
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ bool: TRUE if visible, FALSE if otherwise
	********************************************************************************/
	bool IsVisible();

	/********************************************************************************
	*	--- Function:
	*	SetVisibility(bool)
	*	This function will set this mesh component's visibility
	*
	*	--- Parameters:
	*	@ bool: TRUE if visible, FALSE if otherwise
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetVisibility(bool visible);

	/********************************************************************************
	*	--- Function:
	*	GetTransform()
	*	This function will return if  the game object's transform
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ Matrix4*: cached pointer to game object's transform	
	********************************************************************************/
	Matrix4* GetTransform();

	/********************************************************************************
	*	--- Function:
	*	Draw(Renderer*)
	*	This function will draw this mesh component
	*
	*	--- Parameters:
	*	@ Renderer*: pointer to renderer
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Draw(Renderer* renderer);

private:

	MeshData*								m_pMeshData;	// pointer to mesh data
	Material								m_Material;		// material
	Matrix4*								m_pTransform;	// cached pointer to game object's transform	
	bool									m_bVisible;		// Visibility flag
};

};

#endif // !MeshComponent_H_
// GameObject.h: the class for a game object containing mesh and other data

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../Math/simdmath.h"
#include "../Graphics/MeshComponent.h"

typedef SIMDQuaternion Quaternion;
typedef SIMDVector3 Vector3;
typedef SIMDMatrix4 Matrix4;

class GameObject
{
public:
	GameObject()
		:m_pMeshComponent(nullptr)
	{};

private:
	
	Matrix4*								m_pTransform;

	MeshComponent*							m_pMeshComponent;

	// PhysComponent*						m_pPhysComponent;
};

#endif
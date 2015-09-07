// GameObject.h: the class for a game object containing mesh and other data

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../Math/simdmath.h"
#include "../Graphics/MeshInstance.h"

typedef SIMDQuaternion Quaternion;
typedef SIMDVector3 Vector3;

class GameObject
{
public:
	GameObject()
		:m_pMeshInstance(nullptr)
	{};

private:
	
	MeshInstance*							m_pMeshInstance;
};

#endif
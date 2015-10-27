// MeshComponent.h: the class for a mesh instance which contains a mesh data

#ifndef MeshComponent_H
#define MeshComponent_H

#include "../Math/simdmath.h"

typedef SIMDQuaternion Quaternion;
typedef SIMDVector3 Vector3;

class MeshData;

class MeshComponent
{
public:
	MeshComponent(const char* filename);

	void Draw();

	// temp
	MeshData*								m_pMeshData;

private:
	// Contains all buffer and shaders data
	// MeshData*								m_pMeshData;

	// Quaternion
	Quaternion								m_quat;

	// Position vector
	Vector3									m_pos;
	
};

#endif
// MeshInstance.h: the class for a mesh instance which contains a mesh data

#ifndef MESHINSTANCE_H
#define MESHINSTANCE_H

#include "../Math/simdmath.h"

typedef SIMDQuaternion Quaternion;
typedef SIMDVector3 Vector3;

class MeshData;

class MeshInstance
{
public:
	MeshInstance(const char* filename);

	void Draw();

private:
	// Contains all buffer and shaders data
	MeshData* m_pMeshData;

	// Quaternion
	Quaternion m_quat;

	// Position vector
	Vector3 m_pos;
	
};

#endif
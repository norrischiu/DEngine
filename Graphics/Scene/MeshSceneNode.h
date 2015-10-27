// MeshSceneNode.h:

#ifndef MESHSCENENODE_H_
#define MESHSCENENODE_H_

#include "SceneNode.h"

class MeshSceneNode : public SceneNode
{
public:

	// Overload constructor
	MeshSceneNode(const char* dbgname, MeshData* meshData)
		: SceneNode(dbgname, meshData)
	{};

private:

};

#endif

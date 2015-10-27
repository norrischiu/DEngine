// SceneNode.h:

#ifndef SCENENODE_H_
#define SCENENODE_H_

#include "../Math/simdmath.h"
#include "../MeshData.h"
#include <vector>

typedef SIMDMatrix4 Matrix4;

class SceneNode
{
public:

	// Default constructor
	SceneNode() {};

	// Overload constructor
	SceneNode(const char* dbgname, MeshData* meshData)
		: m_sDebugName(dbgname)
		, m_pMeshData(meshData)
		, m_bDirtyFlag(false)
	{
		m_mLocalTransform = Matrix4::Identity; // temp
		m_mWorldTransform = Matrix4::Identity; // temp
	};

	// Add a scene node to the children list
	void AttachSubNode(std::shared_ptr<SceneNode> subNode);

	// Set world and local transform
	void SetTransform(Matrix4 world, Matrix4 local);

	// Render this node
	void Render(bool flag);

	// Render sub nodes
	void RenderSubNodes();

	// Parent node
	SceneNode*								m_pParent;

	// Sub nodes
	std::vector<std::shared_ptr<SceneNode>>	m_SubNodesList;

private:

	// Debug name
	const char*								m_sDebugName;

	// World to local transform
	Matrix4									m_mLocalTransform;
	
	// Local to world transform
	Matrix4									m_mWorldTransform;

	// Dirty flag to update transform
	bool									m_bDirtyFlag;

	// 
	MeshData*								m_pMeshData;

	// m_ActorId
};

#endif
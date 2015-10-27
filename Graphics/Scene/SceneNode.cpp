#include "SceneNode.h"

void SceneNode::AttachSubNode(std::shared_ptr<SceneNode> subNode)
{
	m_SubNodesList.emplace_back(subNode);
	subNode->m_pParent = this;
}

void SceneNode::SetTransform(Matrix4 world, Matrix4 local)
{
	m_mWorldTransform = world;
	m_mLocalTransform = local;
	m_bDirtyFlag = true;
}

void SceneNode::Render(bool flag)
{
	m_bDirtyFlag |= flag;
	if (m_bDirtyFlag)
	{
		//m_mWorldTransform = m_pParent->m_mWorldTransform * m_mWorldTransform;
		m_mWorldTransform *= m_pParent->m_mWorldTransform;
		m_bDirtyFlag = false;
	}
	m_pMeshData->Render(m_mWorldTransform);
}

void SceneNode::RenderSubNodes()
{
	for (std::shared_ptr<SceneNode> sn : m_SubNodesList)
	{
		sn->RenderSubNodes();
		sn->Render(m_bDirtyFlag);
	}
}

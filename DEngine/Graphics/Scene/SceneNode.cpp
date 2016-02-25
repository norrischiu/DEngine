#include "SceneNode.h"
#include "GameObject\GameWorld.h"

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

void SceneNode::Transform(Matrix4 local)
{
	m_mLocalTransform *= local;
	m_bDirtyFlag = true;
}

void SceneNode::Render(bool flag)
{
	m_bDirtyFlag |= flag;
	if (m_bDirtyFlag)
	{
		m_mWorldTransform = m_mLocalTransform * m_pParent->m_mWorldTransform;
	}
	GameWorld::GetInstance()->GetGameObjectAt(m_iObjectID)->m_pMeshObj->Draw();
}

void SceneNode::RenderSubNodes()
{
	for (std::shared_ptr<SceneNode> sn : m_SubNodesList)
	{
		sn->Render(m_bDirtyFlag);
		sn->RenderSubNodes();
	}
	m_bDirtyFlag = false;
}

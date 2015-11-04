// RootSceneNode.h: a class to manage all children scene node 

#ifndef ROOTSCENENODE_H_
#define ROOTSCENENODE_H_

#include "SceneNode.h"

class RootSceneNode : public SceneNode
{
public:

	// Default constructor
	RootSceneNode()
		: SceneNode("Root Scene Node", nullptr)
	{
		m_pParent = nullptr;
	};

	static RootSceneNode* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new RootSceneNode();
		return m_pInstance;
	}

private:

	// Singleton instance
	static RootSceneNode* m_pInstance;
};

#endif
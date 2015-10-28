#include "GameLoop.h"
#include "../Object/GameObject.h"
#include "../Graphics/Scene/MeshSceneNode.h"
#include "../Graphics/Scene/RootSceneNode.h"

GameLoop* GameLoop::m_pInstance;
static std::shared_ptr<SceneNode> ptr;
GameLoop::GameLoop()
{
	// create a game object with mesh
	// GameObject dragon;
	// or a scene node attach entity

	// attach a mesh component to scene graph
	MeshComponent dragonMC("dragon"); // temp
	MeshSceneNode dragonSN("Dragon Mesh Scene Node", dragonMC.m_pMeshData);
	ptr = std::make_shared<SceneNode>(dragonSN);
	RootSceneNode::getInstance()->AttachSubNode(ptr);
	
	MeshComponent soldierMC("soldier"); // temp
	MeshSceneNode soldierSN("Soldier Mesh Scene Node", soldierMC.m_pMeshData);
	static std::shared_ptr<SceneNode> ptr2 = std::make_shared<SceneNode>(soldierSN);
	ptr->AttachSubNode(ptr2);

	// create a camera
}

void GameLoop::Update(float deltaTime)
{
	Matrix4 temp;
	temp.CreateTranslation(SIMDVector3(1, 0, 0));
	//ptr->Transform(temp);
}

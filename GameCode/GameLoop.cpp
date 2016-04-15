// Debug include
#include "DEngine\DebugRenderer\DEBUG_RENDERER.h"

// Engine include
#include "GameLoop.h"
#include "DEngine\GameObject\GameObject.h"
#include "DEngine\GameObject\GameWorld.h"
#include "DEngine\Light\PointLight.h"
#include "DEngine\Light\SpotLightComponent.h"
#include "DEngine\Object\MovementController.h"
#include "DEngine\Object\Camera.h"
#include "DEngine\Math\simdmath.h"
#include "DEngine\Graphics\HUD\HUD.h"
#include "DEngine\AI\PathFinding\FlowFieldBuilder.h"
#include "DEngine\AI\PathFinding\AIController.h"
#include "DEngine\Graphics\ParticleSystem\ParticleSystem.h"
#include "DEngine\Graphics\Terrain\TerrainBuilder.h"

// Game include
#include "MainPlayer\Player.h"
#include "Boss\Boss.h"

GameLoop* GameLoop::m_pInstance = nullptr;

GameLoop::GameLoop() 
	: m_timer(0.0f) 
{

}

void GameLoop::Construct()
{
//	Terrain* terrain = TerrainBuilder::getInstance()->generateTerrain("terrain.raw", "", 256, 256, 15.0f, 1.0f);
//	GameObject* terrain_gobj = terrain->CreateGameObject("terrain.dds", "terrain_normal.dds", "terrain_height.dds");

//	HUD::getInstance()->addText("timer1", "FYP Progress:", HUDElement::Position(10, 10), HUDElement::FontSize::PT60, HUDElement::Color::RED);
	DE::HUD::getInstance()->addProgress("BossHP", 100.0f, DE::HUDElement::Position(100, 700), DE::HUDElement::Size(800, 20), false);

	Player* player = new Player();
	Boss* boss = new Boss(player);
	boss->SetPosition(DE::Vector3(0.0f, 0.0f, 5.0f));
	player->SetBoss(boss);
	
	for (int i = 0; i < 50; ++i)
	{
		std::string meshName = "church/church" + std::to_string(i);
		DE::GameObject* levelMesh = new DE::GameObject;

		DE::Handle hMeshComponent(sizeof(DE::MeshComponent));
		new (hMeshComponent) DE::MeshComponent(meshName.c_str());
		levelMesh->AddComponent((DE::Component*) hMeshComponent.Raw());

		DE::Handle hAABB(sizeof(DE::AABB));
		new (hAABB) DE::AABB(((DE::MeshComponent*) hMeshComponent.Raw())->m_pMeshData->GetBoundingBox());
		((DE::AABB*) hAABB.Raw())->setType(DE::typeAABB);
		levelMesh->AddComponent((DE::Component*) hAABB.Raw());
	}

	DE::PointLight* light = new DE::PointLight(DE::Vector3(0.0f, 8.0f, 0.0f), DE::Vector4(1.0, 1.0, 1.0), 10.0f, 3.5f);
	DE::Handle hCamera(sizeof(DE::CameraComponent));
	//new (hCamera) DE::CameraComponent(DE::Vector3(0.0f, 0.0f, 0.0f), DE::Vector3(0.0f, -1.0f, 0.0f), DE::Vector3(0.0f, 0.0f, 1.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f);
	//light->AddComponent((DE::Component*)hCamera.Raw());

	new DE::PointLight(DE::Vector3(0.0f, 3.0f, 0.0f), DE::Vector4(1.0f, 0.0, 0.0), 3.5f, 3.0f);
	new DE::PointLight(DE::Vector3(3.0f, 3.0f, 0.0f), DE::Vector4(0.0, 1.0f, 1.0), 3.5f, 3.0f);
	new DE::PointLight(DE::Vector3(-3.0f, 3.0f, 0.0f), DE::Vector4(1.0, 0.0, 1.0f), 3.5f, 3.0f);
}

void GameLoop::Update(float deltaTime)
{
	DE::GameWorld::GetInstance()->Update(deltaTime);
	m_timer += deltaTime;
}
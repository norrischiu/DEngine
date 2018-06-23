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
//#include "DEngine\Graphics\HUD\HUD.h"
#include "DEngine\AI\PathFinding\FlowFieldBuilder.h"
#include "DEngine\AI\PathFinding\AIController.h"
#include "DEngine\Graphics\ParticleSystem\ParticleSystem.h"
#include "DEngine\Graphics\Terrain\TerrainBuilder.h"
#include "DEngine\GlobalInclude.h"

// Game include
#include "MainPlayer\Player.h"
#include "Boss\Boss.h"

#include <string>

GameLoop* GameLoop::m_pInstance = nullptr;

GameLoop::GameLoop() 
	: m_timer(0.0f) 
{

}

void GameLoop::Construct()
{
	Player* player = new Player();
	Boss* boss = new Boss(player);
	boss->SetPosition(DE::Vector3(0.0f, 0.0f, 5.0f));
	player->SetBoss(boss);
	
	for (int i = 0; i < 381; ++i)
	{
		std::string meshName = "church/church" + std::to_string(i);
		DE::GameObject* levelMesh = new DE::GameObject;

		DE::Handle hMeshComponent(sizeof(DE::MeshComponent));
		new (hMeshComponent) DE::MeshComponent(meshName.c_str());
		levelMesh->AddComponent((DE::Component*) hMeshComponent.Raw());

		DE::Handle hAABB(sizeof(DE::AABB));
		new (hAABB) DE::AABB(((DE::MeshComponent*) hMeshComponent.Raw())->GetMeshData()->GetBoundingBox());
		((DE::AABB*) hAABB.Raw())->setType(DE::typeAABB);
		levelMesh->AddComponent((DE::Component*) hAABB.Raw());
	}

	DE::PointLight* light = new DE::PointLight(DE::Vector3(6.0f, 4.0f, 0.0f), DE::Vector4(1.0, 1.0, 1.0), 4.5f, 3.0f, true);
	DE::Handle hCamera(sizeof(DE::CameraComponent));
	new (hCamera) DE::CameraComponent(DE::Vector3(6.0f, 4.0f, 0.0f), DE::Vector3(4.0f, 0.0f, 0.0f), DE::Vector3(0.0f, 0.0f, 1.0f), PI / 2.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 100.0f);
	light->AddComponent((DE::Component*)hCamera.Raw());

	DE::PointLight* light2 = new DE::PointLight(DE::Vector3(0.0f, 4.0f, 0.0f), DE::Vector4(1.0, 1.0, 1.0), 4.5f, 3.0f, true);
	DE::Handle hCamera2(sizeof(DE::CameraComponent));
	new (hCamera2) DE::CameraComponent(DE::Vector3(0.0f, 4.0f, 0.0f), DE::Vector3(0.0f, 0.0f, 0.0f), DE::Vector3(0.0f, 0.0f, 1.0f), PI / 2.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 100.0f);
	light2->AddComponent((DE::Component*)hCamera.Raw());

	DE::PointLight* light3 = new DE::PointLight(DE::Vector3(-6.0f, 4.0f, 0.0f), DE::Vector4(1.0, 1.0, 1.0), 4.5f, 3.0f, true);
	DE::Handle hCamera3(sizeof(DE::CameraComponent));
	new (hCamera3) DE::CameraComponent(DE::Vector3(-6.0f, 4.0f, 0.0f), DE::Vector3(-4.0f, 0.0f, 0.0f), DE::Vector3(0.0f, 0.0f, 1.0f), PI / 2.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 100.0f);
	light3->AddComponent((DE::Component*)hCamera.Raw());

	new DE::PointLight(DE::Vector3(0.0f, 3.0f, 0.0f), DE::Vector4(1.0f, 0.0, 0.0), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(0.0f, 3.0f, 2.0f), DE::Vector4(1.0f, 0.0, 0.0), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(0.0f, 3.0f, -2.0f), DE::Vector4(1.0f, 0.0, 0.0), 3.5f, 3.5f);

	for (int i = 3; i < 27; i+=3)
	{
		new DE::PointLight(DE::Vector3(i * 1.0f, 3.0f, 0.0f), DE::Vector4(0.0, 1.0f, 1.0), 3.5f, 3.5f);
		new DE::PointLight(DE::Vector3(i * -1.0f, 3.0f, 0.0f), DE::Vector4(1.0, 0.0, 1.0f), 3.5f, 3.5f);
		new DE::PointLight(DE::Vector3(i * 1.0f, 3.0f, 2.0f), DE::Vector4(0.0, 1.0f, 1.0), 3.5f, 3.5f);
		new DE::PointLight(DE::Vector3(i * -1.0f, 3.0f, 2.0f), DE::Vector4(1.0, 0.0, 1.0f), 3.5f, 3.5f);
		new DE::PointLight(DE::Vector3(i * 1.0f, 3.0f, -2.0f), DE::Vector4(0.0, 1.0f, 1.0), 3.5f, 3.5f);
		new DE::PointLight(DE::Vector3(i * -1.0f, 3.0f, -2.0f), DE::Vector4(1.0, 0.0, 1.0f), 3.5f, 3.5f);
	}

	// two rows
	new DE::PointLight(DE::Vector3(17.0f, 9.0f, -4.5f), DE::Vector4(1.0, 0.0, 0.0f), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(7.0f, 9.0f, -4.5f), DE::Vector4(1.0, 0.0, 0.0f), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(-3.0f, 9.0f, -4.5f), DE::Vector4(1.0, 0.0, 0.0f), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(-13.0f, 9.0f, -4.5f), DE::Vector4(1.0, 0.0, 0.0f), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(-23.0f, 9.0f, -4.5f), DE::Vector4(1.0, 0.0, 0.0f), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(17.0f, 9.0f, 4.5f), DE::Vector4(1.0, 0.0, 0.0f), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(7.0f, 9.0f, 4.5f), DE::Vector4(1.0, 0.0, 0.0f), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(-3.0f, 9.0f, 4.5f), DE::Vector4(1.0, 0.0, 0.0f), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(-13.0f, 9.0f, 4.5f), DE::Vector4(1.0, 0.0, 0.0f), 3.5f, 3.5f);
	new DE::PointLight(DE::Vector3(-23.0f, 9.0f, 4.5f), DE::Vector4(1.0, 0.0, 0.0f), 3.5f, 3.5f);
}

void GameLoop::Update(float deltaTime)
{
	DE::GameWorld::GetInstance()->Update(deltaTime);
	m_timer += deltaTime;
}
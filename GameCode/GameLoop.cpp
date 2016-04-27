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
#include "DEngine\AI\PathFinding\PositioningSystem.h"
#include "DEngine\AI\PathFinding\FlowFieldBuilder.h"
#include "DEngine\Graphics\Skydome\SkydomeBuilder.h"
#include "DEngine\AI\PathFinding\AIController.h"
#include "DEngine\Graphics\ParticleSystem\ParticleSystem.h"
#include "DEngine\Graphics\Terrain\TerrainBuilder.h"
#include "DEngine\GameObject\GameObjectSpawner.h"
#include "DEngine\GameObject\SpawnManager.h"
#include "MainPlayer\PlayerGOS.h"

// Game include
#include "MainPlayer\Player.h"

GameLoop* GameLoop::m_pInstance = nullptr;

GameLoop::GameLoop()
{

}

void GameLoop::Construct()
{
	/*
	DE::GameObject* skydome = DE::SkydomeBuilder::getInstance()->CreateGameObject();
	DE::Matrix4 transform;
	transform.CreateTranslation(DE::Vector3(0.0f, 30.0f, 5.0f));
	skydome->TransformBy(transform);
	transform.CreateScale(33.75f);
	skydome->TransformBy(transform);
	skydome->GetComponent<DE::MeshComponent>()->m_pMeshData->SetBoundingBox(DE::AABB(DE::Vector3(-127.0f, 0.0f, -127.0f), DE::Vector3(128.0f, 10.0f, 128.0f)));
	*/

	DE::Terrain* terrain = DE::TerrainBuilder::getInstance()->generateTerrain("terrain_height", 256, 256, 15.0f, 1.0f, 64);
	DE::GameObject* terrain_gobj = terrain->CreateGameObject("terrain_diffuse", "terrain_normal");

	DE::GameObject* pointlight = new DE::GameObject;
	pointlight->SetPosition(DE::Vector4(0.0f, 0.0f, 0.0f));
	DE::Handle hPointLight(sizeof(DE::PointLightComponent));
	new (hPointLight) DE::PointLightComponent(DE::Vector4(128.0f, 370.0, 128.0), DE::Vector4(1, 1, 1), 800, 2);
	pointlight->AddComponent((DE::Component*)hPointLight.Raw());

	//DE::HUD::getInstance()->addText("timer1", "Timer: ", DE::HUDElement::Position(10, 10), DE::HUDElement::FontSize::PT60, DE::HUDElement::Color::RED);
	DE::HUD::getInstance()->addText("debug1", "", DE::HUDElement::Position(10, 450), DE::HUDElement::FontSize::PT60, DE::HUDElement::Color::RED);
	DE::HUD::getInstance()->addText("debug2", "", DE::HUDElement::Position(10, 400), DE::HUDElement::FontSize::PT60, DE::HUDElement::Color::RED);
	//DE::HUD::getInstance()->addProgress("progress1", 67.0f, DE::HUDElement::Position(300, 10), DE::HUDElement::Size(500, 100), true);

	Player* player = new Player();
	player->SetPosition(DE::Vector3(0.0f, terrain->GetHeight(0.0f, 50.f), 50.0f));

	/*
	DE::Handle hCamera(sizeof(DE::CameraComponent));
	new (hCamera) DE::CameraComponent(DE::Vector3(0.0f, 22.0f, 1.0f), DE::Vector3(0.0f, 0.0f, 0.0f), DE::Vector3::UnitY, PI / 2.0f, 1024.0f / 768.0f, 1.0f, 1000.0f);
	player->AddComponent((DE::Component*) hCamera.Raw());
	player->GetComponent<DE::CameraComponent>()->SetAsRendererCamera();
	*/

	DE::Camera* cam = new DE::Camera(DE::Vector3(0.0f, 22.0f, -8.0f), DE::Vector3(0.0f, 0.0f, -9.0f), DE::Vector3::UnitY, PI / 2.0f, 1024.0f / 768.0f, 1.0f, 1000.0f);
	cam->SetAsRendererCamera();

	std::vector<DE::GameObject*> obstacles;
	DE::FlowField* flowField_left = DE::FlowFieldBuilder::getInstance()->generateFlowField(DE::Vector3(-127.0f, 0.0f, -127.0f), DE::Vector3(127.0f, 0.0f, 127.0f), obstacles, DE::Vector3(18.0f, 0.0f, -18.0f));
	DE::FlowField* flowField_right = DE::FlowFieldBuilder::getInstance()->generateFlowField(DE::Vector3(-127.0f, 0.0f, -127.0f), DE::Vector3(127.0f, 0.0f, 127.0f), obstacles, DE::Vector3(-18.0f, 0.0f, -18.0f));
	//DE::PositioningSystem::GetInstance()->Init(DE::Vector3(-127.0f, 0.0f, -127.0f), DE::Vector3(127.0f, 0.0f, 127.0f));
	//flowField->Draw(terrain);
	DE::Handle hAIController_left(sizeof(DE::AIController));
	new (hAIController_left) DE::AIController(DE::Vector3(18.0f, 0.0f, -18.0f), terrain);
	//new (hAIController_left) DE::AIController(flowField_left, terrain);
	//player->AddComponent((DE::Component*) hAIController_left.Raw());

	DE::Handle hAIController_right(sizeof(DE::AIController));
	new (hAIController_right) DE::AIController(DE::Vector3(-18.0f, 0.0f, -18.0f), terrain);
	//new (hAIController_right) DE::AIController(flowField_right, terrain);

	PlayerGOS* playerGOS_left = new PlayerGOS(
		new DE::SpawnConfig_Area(
			player,
			49,
			0.1f,
			DE::Vector3(-18.0f, 0.0f, -18.0f),
			DE::Vector3(-10.0f, 0.0f, -10.0f),
			DE::Vector3(2.0f, 0.0f, 2.0f)
		),
		DE::SpawnConfigType::SPAWN_CONFIG_AREA,
		terrain
	);
	playerGOS_left->AddOverrideComponent((DE::Component*) hAIController_left.Raw());
	DE::SpawnManager::GetInstance()->AddSpawner(playerGOS_left);


	PlayerGOS* playerGOS_right = new PlayerGOS(
		new DE::SpawnConfig_Area(
			player,
			49,
			0.1f,
			DE::Vector3(18.0f, 0.0f, -18.0f),
			DE::Vector3(10.0f, 0.0f, -10.0f),
			DE::Vector3(-2.0f, 0.0f, 2.0f)
		),
		DE::SpawnConfigType::SPAWN_CONFIG_AREA,
		terrain
	);
	playerGOS_right->AddOverrideComponent((DE::Component*) hAIController_right.Raw());
	DE::SpawnManager::GetInstance()->AddSpawner(playerGOS_right);

	/*
	DE::SpawnManager::GetInstance()->AddSpawner(new PlayerGOS(
		new DE::SpawnConfig_Area(
		player,
		49,
		0.1f,
		DE::Vector3(18.0f, 0.0f, 18.0f),
		DE::Vector3(6.0f, 0.0f, 6.0f),
		DE::Vector3(-4.0f, 0.0f, -4.0f)
		),
		DE::SpawnConfigType::SPAWN_CONFIG_AREA,
		terrain
	));

	DE::SpawnManager::GetInstance()->AddSpawner(new PlayerGOS(
		new DE::SpawnConfig_Area(
		player,
		49,
		0.1f,
		DE::Vector3(-18.0f, 0.0f, 18.0f),
		DE::Vector3(-6.0f, 0.0f, 6.0f),
		DE::Vector3(4.0f, 0.0f, -4.0f)
		),
		DE::SpawnConfigType::SPAWN_CONFIG_AREA,
		terrain
	));
	*/

	//GameObject* spotlight = new GameObject;
	//spotlight->SetPosition(Vector3(1.0f, 3.0f, 0.0f));
	//spotlight->AddComponent(new SpotLightComponent(spotlight->GetPosition(), Vector3(0.0, 0.0, 0.0) - spotlight->GetPosition(), PI / 10.0f, PI / 18.0f, Vector4(1.0, 1.0, 1.0), 2, 8, false));
	//spotlight->AddComponent(new CameraComponent(spotlight->GetPosition(), Vector3(0.0, 0.0, 0.0) - spotlight->GetPosition(), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f));
}

void GameLoop::Update(float deltaTime)
{
	static bool hasAIInited = false;
	static float m_timer = 0.0f;
	m_timer += deltaTime;

	DE::GameWorld::GetInstance()->Update(deltaTime);
	DE::SpawnManager::GetInstance()->Update(deltaTime);

	if (!hasAIInited)
	{
		if (DE::SpawnManager::GetInstance()->GetSpawnState() == DE::SpawnManager::SpawnState::SPAWN_FINISHED)
		{
			for (auto itr : *DE::GameWorld::GetInstance()->GetGameObjectList())
			{
				DE::AIController* aiController = itr->GetComponent<DE::AIController>();

				if (aiController)
				{
					aiController->Init();
				}
			}
			hasAIInited = true;
		}
	}
	else
	{
		for (auto itr : *DE::GameWorld::GetInstance()->GetGameObjectList())
		{
			DE::AIController* aiController = itr->GetComponent<DE::AIController>();

			if (aiController)
			{
				aiController->Move(deltaTime);
			}
		}
	}

	//((DE::TextBox*) DE::HUD::getInstance()->getHUDElementById("timer1"))->setText("Timer: %.1f", m_timer);
}
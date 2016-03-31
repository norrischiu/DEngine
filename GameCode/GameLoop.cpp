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

GameLoop* GameLoop::m_pInstance = nullptr;

GameLoop::GameLoop()
	: m_timer(0.0f)
{

}

void GameLoop::Construct()
{
	//real-world terrain can be obtained at: http://terrain.party/
	DE::Terrain* terrain = DE::TerrainBuilder::getInstance()->generateTerrain("terrain.raw", "", 256, 256, 15.0f, 1.0f);
	DE::GameObject* terrain_gobj = terrain->CreateGameObject("terrain.dds", "terrain_normal.dds", "terrain_height.dds");

	DE::GameObject* pointlight = new DE::GameObject;
	pointlight->SetPosition(DE::Vector4(0.0, 370.0, 0.0));
	DE::Handle hPointLight(sizeof(DE::PointLightComponent));
	new (hPointLight) DE::PointLightComponent(DE::Vector4(128.0f, 370.0, 128.0), DE::Vector4(1, 1, 1), 400, 50);
	pointlight->AddComponent((DE::Component*)hPointLight.Raw());

	DE::Camera* cam = new DE::Camera(DE::Vector3(0.0f, 70.0f, 10.0f), DE::Vector3(0.0, 10.0, 60.0f), DE::Vector3::UnitY, PI / 2.0f, 1024.0f / 768.0f, 1.0f, 1000.0f);
	cam->SetAsRendererCamera();
	DE::Handle hMC(sizeof(DE::MovementController));
	new (hMC) DE::MovementController(100.0f);
	cam->AddComponent((DE::Component*)hMC.Raw());

	//	HUD::getInstance()->addText("timer1", "FYP Progress:", HUDElement::Position(10, 10), HUDElement::FontSize::PT60, HUDElement::Color::RED);
	//	HUD::getInstance()->addProgress("progress1", 67.0f, HUDElement::Position(300, 10), HUDElement::Size(500, 100), true);

	//Player* player = new Player();
	//player->GetComponent<DE::CameraComponent>()->SetAsRendererCamera();

	//	std::vector<GameObject*> obstacles;
	//	FlowField flowField = FlowFieldBuilder::getInstance()->generateFlowField(floor, obstacles, Vector3(5.0f, 0.0f, 5.0f));
	//	flowField.print();
	//	player->AddComponent(new AIController(flowField));
	//	player->AddComponent(DE::ParticleSystem::GetInstance()->AddParticles("torch_flame_1", 1, DE::Vector3(-2.5f, 0.5f, 0.0f), DE::Vector3(0.0f, 0.0f, 0.0f)));

	//GameObject* spotlight = new GameObject;
	//spotlight->SetPosition(Vector3(1.0f, 3.0f, 0.0f));
	//spotlight->AddComponent(new SpotLightComponent(spotlight->GetPosition(), Vector3(0.0, 0.0, 0.0) - spotlight->GetPosition(), PI / 10.0f, PI / 18.0f, Vector4(1.0, 1.0, 1.0), 2, 8, false));
	//spotlight->AddComponent(new CameraComponent(spotlight->GetPosition(), Vector3(0.0, 0.0, 0.0) - spotlight->GetPosition(), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f));
}

void GameLoop::Update(float deltaTime)
{
	DE::GameWorld::GetInstance()->Update(deltaTime);
	m_timer += deltaTime;
	//	((TextBox*) HUD::getInstance()->getHUDElementById("timer1"))->setText("Timer: %.1f", m_timer);
}
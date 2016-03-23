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
#include "DEngine\Graphics\Terrain\Terrain.h"

// Game include
#include "MainPlayer\Player.h"

using namespace DE;

GameLoop* GameLoop::m_pInstance = nullptr;

GameLoop::GameLoop() 
	: m_timer(0.0f) 
{

}

void GameLoop::Construct()
{
	HUD::getInstance()->addText("text1", "FYP Progress:", HUDElement::Position(10, 10), HUDElement::FontSize::PT60, HUDElement::Color::RED);
	HUD::getInstance()->addText("timer1", "Timer: 0.0", HUDElement::Position(10, 80), HUDElement::FontSize::PT60, HUDElement::Color::RED);
	// HUD::getInstance()->addText("debug1", "", HUDElement::Position(10, 150), HUDElement::FontSize::PT60, HUDElement::Color::RED);
	HUD::getInstance()->addProgress("progress1", 67.0f, HUDElement::Position(300, 10), HUDElement::Size(500, 100), true);

	Camera* cam = new Camera(Vector3(0.0f, 100.0f, -30.0f), Vector3(0.0f, 30.0f, 160.0f), Vector3(0.0f, 1.0f, 0.0f), PI / 2.0f, 1024.0f / 768.0f, 1.0f, 1000.0f);
	cam->SetAsRendererCamera();
	cam->AddComponent(new MovementController());

	//GameObject* spotlight = new GameObject;
	//spotlight->SetPosition(Vector3(1.0f, 3.0f, 0.0f));
	//spotlight->AddComponent(new SpotLightComponent(spotlight->GetPosition(), Vector3(0.0, 0.0, 0.0) - spotlight->GetPosition(), PI / 10.0f, PI / 18.0f, Vector4(1.0, 1.0, 1.0), 2, 8, false));
	//spotlight->AddComponent(new CameraComponent(spotlight->GetPosition(), Vector3(0.0, 0.0, 0.0) - spotlight->GetPosition(), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f));

	//real-world terrain can be obtained at: http://terrain.party/
	Terrain* terrain = TerrainBuilder::getInstance()->generateTerrain("terrain.raw", "", 256, 256, 15.0f, 1.0f);
	GameObject* terrain_gobj = terrain->CreateGameObject("terrain.dds", "terrain_normal.dds");

	GameObject* pointlight = new GameObject;
	pointlight->SetPosition(Vector3(0.0f, 0.0f, 10.0f));
	pointlight->AddComponent(new PointLightComponent(Vector3(0,0,0), Vector4(0.9, 0.9, 0.9), 1000, 100));

	//GameObject dragon = GameObject::Builder().AddComponent(new MeshComponent("dragon")).AddTransform(Matrix4::Identity)
	// GameObject dragon = GameObject::Builder().Components(new MeshComponent("dragon"), new MovementController()).Transform(Matrix4::Identity)

	//	dragon->AddComponent(new Body(typeAABB));
	//dragon->AddComponent(new CameraComponent(Vector3(0.0f, 4.0f, -5.0f), Vector3(0.0f, 0.0f, 1000.0f), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f));
	//dragon->GetComponent<CameraComponent>()->SetAsRendererCamera();

	//GameObject* floor = new GameObject;
	//floor->AddComponent(new MeshComponent("floor"));

	Player* player = new Player();
	//player->SetPosition(Vector3(-7.0f, terrain->GetHeight(-7.0f, -7.0f), -7.0f));
	std::vector<GameObject*> obstacles;

	//FlowField flowField = FlowFieldBuilder::getInstance()->generateFlowField(terrain_gobj, obstacles, Vector3(4.9f, 0.0f, 4.9f));
	FlowField flowField = FlowFieldBuilder::getInstance()->generateFlowField(Vector3(-7.0f, 0.0f, -7.0f), Vector3(7.0f, 0.0f, 7.0f), obstacles, Vector3(6.9f, 0.0f, 6.9f));
	//flowField.Draw();
//	player->AddComponent(new AIController(flowField, terrain));
	//player->AddComponent(DE::ParticleSystem::GetInstance()->AddParticles("torch_flame_1", 1, DE::Vector3(-2.5f, 0.5f, 0.0f), DE::Vector3(0.0f, 0.0f, 0.0f)));
	//player->AddComponent(new Emitter("torch_flame", 4, 6.0f, Vector3(0.0f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)));
}

void GameLoop::Update(float deltaTime)
{
	GameWorld::GetInstance()->Update(deltaTime);
	m_timer += deltaTime;
	((TextBox*) HUD::getInstance()->getHUDElementById("timer1"))->setText("Timer: %.1f", m_timer);
}

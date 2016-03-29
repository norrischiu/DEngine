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
#include "DEngine\Memory\Handle.h"

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
	HUD::getInstance()->addText("debug1", "", HUDElement::Position(10, 150), HUDElement::FontSize::PT60, HUDElement::Color::RED);
	HUD::getInstance()->addProgress("progress1", 67.0f, HUDElement::Position(300, 10), HUDElement::Size(500, 100), true);

	Camera* cam = new Camera(Vector3(0.0f, 70.0f, -30.0f), Vector3(0.0f, 0.0f, 30.0f), Vector3(0.0f, 1.0f, 0.0f), PI / 2.0f, 1024.0f / 768.0f, 1.0f, 1000.0f);
	cam->SetAsRendererCamera();
	Handle hMC(sizeof(MovementController));
	new (hMC) MovementController(100.0f);
	cam->AddComponent((Component*)hMC.Raw());

	//real-world terrain can be obtained at: http://terrain.party/
	Terrain* terrain = TerrainBuilder::getInstance()->generateTerrain("terrain.raw", "", 256, 256, 15.0f, 1.0f);
	GameObject* terrain_gobj = terrain->CreateGameObject("terrain.dds", "terrain_normal.dds", "terrain_height.dds");

	GameObject* pointlight = new GameObject;
	pointlight->SetPosition(Vector3(0.0f, 800.0f, 0.0f));
	Handle hPointLight(sizeof(PointLight));
	new (hPointLight) PointLightComponent(Vector3(0, 0, 0), Vector4(0.9, 0.9, 0.9), 1000, 1000);
	pointlight->AddComponent((Component*) hPointLight.Raw());
}

void GameLoop::Update(float deltaTime)
{
	GameWorld::GetInstance()->Update(deltaTime);
	m_timer += deltaTime;
//	((TextBox*) HUD::getInstance()->getHUDElementById("timer1"))->setText("Timer: %.1f", m_timer);
}

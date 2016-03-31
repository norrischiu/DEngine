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
//	Terrain* terrain = TerrainBuilder::getInstance()->generateTerrain("terrain.raw", "", 256, 256, 15.0f, 1.0f);
//	GameObject* terrain_gobj = terrain->CreateGameObject("terrain.dds", "terrain_normal.dds", "terrain_height.dds");

	/*GameObject* pointlight = new GameObject;
	pointlight->SetPosition(Vector3(3.0f, 3.0f, 0.0f));
	DE::Handle hPointLight(sizeof(PointLightComponent));
	new (hPointLight) PointLightComponent(Vector4(0.0, 0.0, 0.0), Vector4(0.5, 0.5, 0.0), 5, 3, false);
	pointlight->AddComponent((Component*)hPointLight.Raw());*/

//	HUD::getInstance()->addText("timer1", "FYP Progress:", HUDElement::Position(10, 10), HUDElement::FontSize::PT60, HUDElement::Color::RED);
//	HUD::getInstance()->addProgress("progress1", 67.0f, HUDElement::Position(300, 10), HUDElement::Size(500, 100), true);

	Player* player = new Player();
	
	for (int i = 0; i < 50/*381*/; ++i)
	{
		std::string meshName = "church/church" + std::to_string(i);
		DE::GameObject* levelMesh = new DE::GameObject;
		DE::Handle hLevelMesh(sizeof(DE::MeshComponent));
		new (hLevelMesh) DE::MeshComponent(meshName.c_str());
		levelMesh->AddComponent((DE::Component*) hLevelMesh.Raw());
	}
}

void GameLoop::Update(float deltaTime)
{
	DE::GameWorld::GetInstance()->Update(deltaTime);
	m_timer += deltaTime;
}
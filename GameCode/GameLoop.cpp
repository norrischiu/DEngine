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

// Game include
#include "MainPlayer\Player.h"

GameLoop* GameLoop::m_pInstance = nullptr;

GameLoop::GameLoop()
{
	// GameObject dragon = GameObject::Builder().AddComponent(new MeshComponent("dragon")).AddTransform(Matrix4::Identity)
	// GameObject dragon = GameObject::Builder().Components(new MeshComponent("dragon"), new MovementController()).Transform(Matrix4::Identity)

	Player* player = new Player();

	//	dragon->AddComponent(new Body(typeAABB));
	//dragon->AddComponent(new CameraComponent(Vector3(0.0f, 4.0f, -5.0f), Vector3(0.0f, 0.0f, 1000.0f), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f));
	//dragon->GetComponent<CameraComponent>()->SetAsRendererCamera();

	GameObject* floor = new GameObject;
	floor->AddComponent(new MeshComponent("floor"));

	Camera* cam = new Camera(Vector3(0.0f, 2.0f, -7.0f), Vector3(0.0f, 2.0f, 3.0f), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f);
	cam->SetAsRendererCamera();

	GameObject* pointlight = new GameObject;
	pointlight->SetPosition(Vector3(0.0f, 3.0f, -3.0f));
	pointlight->AddComponent(new PointLightComponent(pointlight->GetPosition(), Vector4(0.5, 0.5, 0.0), 8, 8));

	//GameObject* spotlight = new GameObject;
	//spotlight->SetPosition(Vector3(1.0f, 3.0f, 0.0f));
	//spotlight->AddComponent(new SpotLightComponent(spotlight->GetPosition(), Vector3(0.0, 0.0, 0.0) - spotlight->GetPosition(), PI / 10.0f, PI / 18.0f, Vector4(1.0, 1.0, 1.0), 2, 8, false));
	//spotlight->AddComponent(new CameraComponent(spotlight->GetPosition(), Vector3(0.0, 0.0, 0.0) - spotlight->GetPosition(), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f));

	HUD::getInstance()->addText("text1", "Text", HUDElement::Position(10, 10), HUDElement::FontSize::PT60, HUDElement::Color::RED);
	HUD::getInstance()->addProgress("progress1", 20.0f, HUDElement::Position(300, 10), HUDElement::Size(500, 100));

	((TextBox*) HUD::getInstance()->getHUDElementById("text1"))->setText("FYP Progress:");
	((ProgressBar*) HUD::getInstance()->getHUDElementById("progress1"))->setProgress(100.0f);
}

void GameLoop::Update(float deltaTime)
{
	GameWorld::GetInstance()->Update(deltaTime);
}
/*
void GameLoop::KeyboardEventHandeler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

if (msg == WM_KEYDOWN) {
if (GetAsyncKeyState(VK_1)) {
D3D11Renderer::GetInstance()->SetCamera(CameraType::MOVE_CAMERA);
}
else if (GetAsyncKeyState(VK_2)) {
D3D11Renderer::GetInstance()->SetCamera(CameraType::DEGREE360_CAMERA);
}
else if (GetAsyncKeyState(VK_3)) {
D3D11Renderer::GetInstance()->SetCamera(CameraType::TOP_VIEW_CAMERA);
}
else if (GetAsyncKeyState(VK_4)) {
D3D11Renderer::GetInstance()->SetCamera(CameraType::BOTTOM_VIEW_CAMERA);
}
else if (GetAsyncKeyState(VK_5)) {
D3D11Renderer::GetInstance()->SetCamera(CameraType::LEFT_VIEW_CAMERA);
}
else if (GetAsyncKeyState(VK_6)) {
D3D11Renderer::GetInstance()->SetCamera(CameraType::RIGHT_VIEW_CAMERA);
}
else if (GetAsyncKeyState(VK_7)) {
D3D11Renderer::GetInstance()->SetCamera(CameraType::FRONT_VIEW_CAMERA);
}
else if (GetAsyncKeyState(VK_8)) {
D3D11Renderer::GetInstance()->SetCamera(CameraType::BACK_VIEW_CAMERA);
}

if (D3D11Renderer::GetInstance()->GetCameraType() == CameraType::MOVE_CAMERA) {
if (GetAsyncKeyState(VK_W) || GetAsyncKeyState(VK_S)) {
D3D11Renderer::GetInstance()->GetCamera()->move(
GetAsyncKeyState(VK_W) ? CameraMove::FORWARD : CameraMove::BACKWARD, 0.4f
);
}
else if (GetAsyncKeyState(VK_A) || GetAsyncKeyState(VK_D)) {
D3D11Renderer::GetInstance()->GetCamera()->move(
GetAsyncKeyState(VK_A) ? CameraMove::LEFT : CameraMove::RIGHT, 0.4f
);
}
else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_RIGHT)) {
D3D11Renderer::GetInstance()->GetCamera()->rotateVLookAt(
GetAsyncKeyState(VK_LEFT) ? CameraMove::ROTATE_LEFT : CameraMove::ROTATE_RIGHT, 0.03f
);
}
else if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) {
D3D11Renderer::GetInstance()->GetCamera()->rotateVLookAt(
GetAsyncKeyState(VK_DOWN) ? CameraMove::ROTATE_DOWN : CameraMove::ROTATE_UP, 0.03f
);
}
}
}

}*/
#include "GameLoop.h"
#include "Graphics/MeshComponent.h"
#include "Debug\Debug.h"
#include "Font\Font.h"
#include "Object\ObjectLoader.h"
#include "Physics\cdSphere.h"
#include "Physics\cdObject.h"
#include "Physics\cdAabb.h"
#include "GameObject\GameObject.h"
#include "GameObject\GameWorld.h"
#include "Light\PointLight.h"
#include "Object\MovementController.h"

GameLoop* GameLoop::m_pInstance;

GameLoop::GameLoop()
{
	MeshComponent* dragonMC = new MeshComponent("dragon"); // temp
	//MeshComponent* face4MC = new MeshComponent("face4"); // temp

	GameObject* dragon = new GameObject(nullptr, dragonMC, Matrix4::Identity, 0);
	//GameObject* face4 = new GameObject(nullptr, face4MC, Matrix4::Identity, 0);
	//face4->AttachTo(0);

	// create a light
	PointLight* testPointLight = new PointLight(Vector3(0.0f, 4.0f, 0.5f), 10, Vector4(0.1, 0.1, 0.1), Vector4(0.5, 0.5, 0.5), Vector4(1, 1, 1), 1);
	//PointLight* testPointLight2 = new PointLight(Vector3(0.0f, 4.0f, 3.0f), 2, Vector4(0.1, 0.1, 0.1), Vector4(0.5, 0.5, 0.5), Vector4(1, 1, 1), 1);
	
	//Camera* cam = new Camera(Vector3(0.0f, 170.0f / 100.0f, -170.0f / 100.0f), Vector3(0.0f, 170.0f / 100.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f);

	Debug debug;
	//MeshComponent* m = debug.draw_prism(Vector3(1.0f, 1.0f, 1.0f), Primitives::RECTANGULAR_PRISM);
	Matrix4 scale;
	//scale.CreateScale(testPointLight2->GetRadius());
	Matrix4 trans;
	//trans.CreateTranslation(testPointLight2->GetPosition());
	//GameObject* lightSphere2 = new GameObject(nullptr, m, trans * scale, 1);
	MeshComponent* m2 = debug.draw_prism(Vector3(1.0f, 1.0f, 1.0f), Primitives::RECTANGULAR_PRISM);
	scale.CreateScale(testPointLight->GetRadius());
	trans.CreateTranslation(testPointLight->GetPosition());
	GameObject* lightbox = new GameObject(nullptr, m2, trans * scale, 1);
	MeshComponent* m3 = debug.draw_ellipsoid(Vector3(0.1f, 0.1f, 0.1f), Primitives::SPHERE, 5);
	trans.CreateTranslation(testPointLight->GetPosition());
	GameObject* lightSphere = new GameObject(nullptr, m3, trans, 2);
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
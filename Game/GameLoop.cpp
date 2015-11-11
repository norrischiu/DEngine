#include "GameLoop.h"
#include "../Graphics/Scene/MeshSceneNode.h"
#include "../Graphics/Scene/RootSceneNode.h"
#include "../Graphics/MeshComponent.h"
#include "..\Debug\Debug.h"
#include "..\Font\Font.h"
#include "..\Object\ObjectLoader.h"
#include "..\Physics\cdSphere.h"
#include "..\Physics\cdObject.h"
#include "..\Physics\cdAabb.h"
#include "..\GameObject\GameObject.h"
#include "..\GameObject\GameWorld.h"

long GameLoop::cursor_pos[2] = { 0, 0 };
typedef SIMDVector3 Vector3;

GameLoop* GameLoop::m_pInstance;

GameLoop::GameLoop()
{
	MeshComponent* dragonMC = new MeshComponent("dragon"); // temp
	MeshComponent* soldierMC = new MeshComponent("soldier"); // temp

	GameObject* dragon = new GameObject(nullptr, dragonMC, Matrix4::Identity, 0);
	GameObject* solider = new GameObject(nullptr, soldierMC, Matrix4::Identity, 1);
	solider->AttachTo(0);

	// create a camera
	//Debug debug;
	//MeshComponent* m = debug.draw_ellipsoid(Vector3(2.0f, 2.0f, 2.0f), Primitives::CONE, 30);
	//m->m_pMeshData->Transform(0.5f, Vector3(0, 0, 0), Vector3(0, 0, 0));
	//
}

void GameLoop::Update(float deltaTime)
{
	
	Matrix4 temp;
	temp.CreateTranslation(SIMDVector3(0.01, 0, 0));
	GameWorld::GetInstance()->GetGameObjectAt(0)->Transform(temp); //
	GameWorld::GetInstance()->Update(deltaTime);
	
}

void GameLoop::SetCursorPosition(long x, long y) {
	cursor_pos[0] = x;
	cursor_pos[1] = y;
}

void GameLoop::MouseEventHandeler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static bool firstMove = false;
	if (!firstMove) {
		SetCursorPosition(LOWORD(lParam), HIWORD(lParam));
		firstMove = true;
		return;
	}

	if (msg == WM_MOUSEMOVE &&
		D3D11Renderer::GetInstance()->GetCameraType() == CameraType::DEGREE360_CAMERA &&
		(GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON))
		) {
		D3D11Renderer::GetInstance()->GetCamera()->rotateVPos(
			(LOWORD(lParam) - cursor_pos[0]) / 150.0f,
			(HIWORD(lParam) - cursor_pos[1]) / 150.0f
			);
	}

	SetCursorPosition(LOWORD(lParam), HIWORD(lParam));
}

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
}
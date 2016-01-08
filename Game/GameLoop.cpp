#include "GameLoop.h"
#include "Graphics\MeshComponent.h"
#include "Debug\Debug.h"
#include "Font\Font.h"
#include "Object\ObjectLoader.h"
#include "GameObject\GameObject.h"
#include "GameObject\GameWorld.h"
#include "Light\PointLight.h"
#include "Object\MovementController.h"
#include "Object\Camera.h"

GameLoop* GameLoop::m_pInstance;

GameLoop::GameLoop()
{ 
	// GameObject dragon = GameObject::Builder().AddComponent(new MeshComponent("dragon")).AddTransform(Matrix4::Identity)
	// GameObject dragon = GameObject::Builder().Components(new MeshComponent("dragon"), new MovementController()).Transform(Matrix4::Identity)

	GameObject* dragon = new GameObject;
	//dragon->AddComponent(new MovementController());
	dragon->AddComponent(new MeshComponent("dragon"));
	dragon->AddComponent(new Body(typeAABB));
	//dragon->AddComponent(new CameraComponent(Vector3(0.0f, 10.0f, -10.0f), Vector3(0.0f, 1.7f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f));
	//dragon->GetComponent<CameraComponent>()->SetAsRendererCamera();

	GameObject* floor = new GameObject;
	floor->AddComponent(new MeshComponent("floor"));

	Camera* cam = new Camera(Vector3(0.0f, 10.0f, 8.0f), Vector3(0.0f, 1.7f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f);
	cam->SetAsRendererCamera();

	GameObject* pointlight = new GameObject;
	pointlight->SetPosition(Vector3(0.0f, 5.0f, 5.0f));
	pointlight->AddComponent(new PointLightComponent(pointlight->GetPosition(), 10, Vector4(0.1, 0.1, 0.1), Vector4(0.5, 0.5, 0.5), Vector4(1, 1, 1), 80));
	pointlight->AddComponent(new CameraComponent(pointlight->GetPosition(), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), PI / 4.0f, 1024.0f / 768.0f, 1.0f, 100.0f));
	
	/*
	* Use this function to set the camera from the light perspective,
	* or in any other file, use:
	* GameObject* pointlight = LightManager::GetInstance()->GetLightAt(0)->GetOwner();
	* pointLight->GetComponent<CameraComponent>()->SetAsRendererCamera();
	*/
	pointlight->GetComponent<CameraComponent>()->SetAsRendererCamera();

	// temp
	Debug debug;
	Matrix4 trans;
	MeshComponent* m3 = debug.draw_ellipsoid(Vector3(0.1f, 0.1f, 0.1f), Primitives::SPHERE, 5);
	trans.CreateTranslation(pointlight->GetComponent<PointLightComponent>()->GetPosition());
	GameObject* lightSphere = new GameObject();
	lightSphere->AddComponent(m3);
	lightSphere->setTransform(trans);
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
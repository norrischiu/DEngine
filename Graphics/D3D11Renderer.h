// D3D11Renderer.h: the class for Direct3D 11 renderer
#ifndef D3D11RENDERER_H
#define D3D11RENDERER_H

#include <D3D11.h>
#include <windows.h>
#include <vector>
#include "../Math/simdmath.h"
#include "MeshComponent.h"

#pragma comment (lib, "D3D11")

typedef SIMDMatrix4 Matrix4;
typedef SIMDVector3 Vector3;

// Singleton of the Direct3D 11 rendering device
class D3D11Renderer
{
public:
	// Default empty constructor
	D3D11Renderer();

	void ConstructWithWindow(HWND hWnd);

	void DestructandCleanUp()
	{
		if (m_pRenderTargetView)
			m_pRenderTargetView->Release();
		if (m_pSwapChain)
			m_pSwapChain->Release();
		if (m_pD3D11Context)
			m_pD3D11Context->Release();
		if (m_pD3D11Device)
			m_pD3D11Device->Release();
	}

	void Update();

	void Render();

	static D3D11Renderer* GetInstance();

	Camera* GetCamera();

	void SetCamera(CameraType cameraType);

	CameraType GetCameraType();

	std::vector<MeshComponent*>& GetMeshComponentList();

	// Pointer to interface, handles GPU and pipeline
	ID3D11DeviceContext*						m_pD3D11Context;

	// Pointer to device, handles video memory
	ID3D11Device*								m_pD3D11Device;

	// Pointer to the double buffer
	IDXGISwapChain*								m_pSwapChain;

	// Pointer to the render target view
	ID3D11RenderTargetView*						m_pRenderTargetView;

	ID3D11DepthStencilView*						m_pDepthStencilView;

private:
	// Singleton instance
	static D3D11Renderer*						m_pInstance;

	// List of all mesh instance to be drawn
	std::vector<MeshComponent*>					m_MeshComponentList;

	Camera*										m_camera;

	CameraType									m_currrent_camera_type;
};

#endif
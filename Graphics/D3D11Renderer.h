// D3D11Renderer.h: the class for Direct3D 11 renderer
#ifndef D3D11RENDERER_H
#define D3D11RENDERER_H

#include <D3D11.h>
#include <windows.h>
#include <vector>
#include "Math\simdmath.h"
#include "GBuffer.h"
#include "Object\CameraComponent.h"

#pragma comment (lib, "D3D11")

class MeshComponent;

// Number of render target in G-Buffer
const static unsigned int					RT_NUM = 3;

// Singleton of the Direct3D 11 rendering device
class D3D11Renderer
{
public:
	// Default empty constructor
	D3D11Renderer();

	void ConstructWithWindow(HWND hWnd);

	void DestructandCleanUp()
	{
		for (int i = 0; i < RT_NUM; i++)
		{
			if (m_pRenderTargetView[i])
				m_pRenderTargetView[i]->Release();
		}

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

	CameraComponent* GetCamera();

	void SetCamera(CameraType cameraType);

	void SetCamera(CameraComponent* camera);

	CameraType GetCameraType();

	std::vector<MeshComponent*>& GetMeshComponentList();

	// Pointer to interface, handles GPU and pipeline
	ID3D11DeviceContext*						m_pD3D11Context;

	// Pointer to device, handles video memory
	ID3D11Device*								m_pD3D11Device;

	// Pointer to the double buffer
	IDXGISwapChain*								m_pSwapChain;

	// Array of pointer to the render target view
	ID3D11RenderTargetView*						m_pRenderTargetView[RT_NUM];

	// Array of pointer to shader resources view
	ID3D11ShaderResourceView*					m_pShaderResourceView[RT_NUM];

	// Array of pointer to texture for deferred use
	ID3D11Texture2D*							m_pTexture[RT_NUM];

	//
	ID3D11RenderTargetView*						m_pBackBufferRTView;

	ID3D11DepthStencilView*						m_pDepthStencilView;
	ID3D11DepthStencilView*						m_pDepthStencilReadOnlyView;

	ID3D11DepthStencilState*					m_pDepthStencilState;

	ID3D11DepthStencilState*					m_pOffDepthStencilState;

	ID3D11DepthStencilState*					m_pOffDepthOffStencilState;

	ID3D11BlendState*							m_pBlendState;

	ID3D11RasterizerState*						m_pCullBackRSState;

	ID3D11RasterizerState*						m_pCullNoneRSState;

	ID3D11RasterizerState*						m_pFillWireRSState;

private:
	// Singleton instance
	static D3D11Renderer*						m_pInstance;

	// List of all mesh instance to be drawn
	std::vector<MeshComponent*>					m_MeshComponentList;

	CameraComponent*							m_camera;

	CameraComponent*							m_renderCamera;

	CameraType									m_currrent_camera_type;

	GBuffer*									m_GBuffer;
};

#endif
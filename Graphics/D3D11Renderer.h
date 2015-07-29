// D3D11Renderer.h: the class for Direct3D 11 renderer
#ifndef D3D11RENDERER_H
#define D3D11RENDERER_H

#include <D3D11.h>
#include <windows.h>
#include <vector>
#include "../Math/simdmath.h"
#include "MeshInstance.h"

#pragma comment (lib, "D3D11")

typedef SIMDMatrix4 Matrix4;
typedef SIMDVector3 Vector3;

// Singleton of the Direct3D 11 rendering device
class D3D11Renderer
{
public:
	// Default empty constructor
	D3D11Renderer(){};

	void ConstructWithWindow(HWND hWnd);

	void DestructandCleanUp()
	{
		if (m_pD3D11Context)
			m_pD3D11Context->Release();
		if (m_pD3D11Device)
			m_pD3D11Device->Release();
		if (m_pSwapChain)
			m_pSwapChain->Release();
		if (m_pRenderTargetView)
			m_pRenderTargetView->Release();
		if (m_pRasterizerState)
			m_pRasterizerState->Release();
	}

	void Update();

	void Render();

	static D3D11Renderer* getInstance()
	{
		if (!m_pInstance)
			m_pInstance = new D3D11Renderer();
		return m_pInstance;
	}

	// Pointer to interface, handles GPU and pipeline
	ID3D11DeviceContext*				m_pD3D11Context;

	// Pointer to device, handles video memory
	ID3D11Device*						m_pD3D11Device;

	// Pointer to the double buffer
	IDXGISwapChain*						m_pSwapChain;

	// Pointer to the render target view
	ID3D11RenderTargetView*				m_pRenderTargetView;

private:
	// Singleton instance
	static D3D11Renderer*				m_pInstance;

	// List of all mesh instance to be drawn
	std::vector<MeshInstance*>			m_MeshInstaceList;

	// Pointer to rasterizer state
	ID3D11RasterizerState*				m_pRasterizerState;

	// Matrics
	Matrix4								m_CameraMat;
	Matrix4								m_ProjectionMat;
};

#endif
// DeferredRenderer.h: the class for Direct3D 11 renderer
#ifndef DeferredRenderer_H
#define DeferredRenderer_H

#include <D3D11.h>
#include <windows.h>
#include <vector>
#include "../Math/simdmath.h"
#include "MeshComponent.h"

#pragma comment (lib, "D3D11")

typedef SIMDMatrix4 Matrix4;
typedef SIMDVector3 Vector3;

// Number of render target in G-Buffer
const static unsigned int					RT_NUM = 1;

// Singleton of the Direct3D 11 rendering device
class DeferredRenderer
{
public:
	// Default empty constructor
	DeferredRenderer(){};

	void ConstructWithWindow(HWND hWnd);

	void DestructandCleanUp()
	{
		if (m_pRenderTargetView)
			m_pRenderTargetView[0]->Release();
		if (m_pSwapChain)
			m_pSwapChain->Release();
		if (m_pD3D11Context)
			m_pD3D11Context->Release();
		if (m_pD3D11Device)
			m_pD3D11Device->Release();
	}

	void Render();

	static DeferredRenderer* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new DeferredRenderer();
		return m_pInstance;
	}

	// Pointer to interface, handles GPU and pipeline
	ID3D11DeviceContext*						m_pD3D11Context;

	// Pointer to device, handles video memory
	ID3D11Device*								m_pD3D11Device;

	// Pointer to the double buffer
	IDXGISwapChain*								m_pSwapChain;

	// Array of pointer to the render target view
	ID3D11RenderTargetView*						m_pRenderTargetView[RT_NUM];

	ID3D11DepthStencilView*						m_pDepthStencilView;

private:
	// Singleton instance
	static DeferredRenderer*					m_pInstance;

	// List of all mesh instance to be drawn
	std::vector<MeshComponent*>					m_MeshComponentList;
};

#endif
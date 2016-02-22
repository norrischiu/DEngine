// D3D11Renderer.h: the class for Direct3D 11 renderer
#ifndef D3D11RENDERER_H_
#define D3D11RENDERER_H_

#include <D3D11.h>
#include <windows.h>
#include "GBuffer.h"
#include "PostProcessEffect.h"
#include "Render\Texture.h"
#include "Object\CameraComponent.h"

#pragma comment (lib, "D3D11")

// Number of render target in G-Buffer
const static unsigned int					RT_NUM = 2;

// Singleton of the Direct3D 11 rendering device
class D3D11Renderer
{

public:

	// Default empty constructor
	D3D11Renderer() {};

	void ConstructWithWindow(HWND hWnd);

	void DestructandCleanUp()
	{
		for (int i = 0; i < RT_NUM; i++)
		{
			//if (m_pRenderTargetView[i])
			//	m_pRenderTargetView[i]->Release();
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

	static D3D11Renderer* GetInstance() 
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new D3D11Renderer();
		}
		return m_pInstance;
	}

	CameraComponent* GetCamera()
	{
		return m_RendererCamera;
	}

	void SetCamera(CameraComponent* camera)
	{
		m_RendererCamera = camera;
	}

	void UnbindRenderTargets()
	{
		m_pD3D11Context->OMSetRenderTargets(0, nullptr, nullptr);
	}

	void UnBindStreamOutTargets()
	{
		ID3D11Buffer* null[1] = { nullptr };
		const unsigned int offset = 0;
		m_pD3D11Context->SOSetTargets(1, null, &offset);
	}

	void UnbindPSShaderResources(int num)
	{
		ID3D11ShaderResourceView* null[5] = { nullptr };
		D3D11Renderer::GetInstance()->m_pD3D11Context->PSSetShaderResources(0, num, null);
	}

	// Pointer to interface, handles GPU and pipeline
	ID3D11DeviceContext*						m_pD3D11Context;

	// Pointer to device, handles video memory
	ID3D11Device*								m_pD3D11Device;

	// Pointer to the double buffer
	IDXGISwapChain*								m_pSwapChain;

	// Array of pointer to the render target view
	ID3D11RenderTargetView*						m_pRTVArray[RT_NUM];

	// Array of pointer to shader resources view
	ID3D11ShaderResourceView*					m_pSRVArray[RT_NUM+1];

	// Array of pointer to texture for deferred use
	Texture*									m_textures[RT_NUM]; // Gbuffer RTV, SRV

	Texture*									m_LightingAccumBuffer;

	Texture*									m_backbuffer; // RTV

	Texture*									m_depth; // DSV, SRV

	Texture*									m_depthReadOnly; // DSV

private:

	// Singleton instance
	static D3D11Renderer*						m_pInstance;

	CameraComponent*							m_RendererCamera;

	GBuffer*									m_GBuffer;

	PostProcessEffect* m_PPE;
};

#endif
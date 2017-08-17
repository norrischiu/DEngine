// D3D11Renderer.h: the class for Direct3D 11 renderer
#ifndef D3D11RENDERER_H_
#define D3D11RENDERER_H_

#include "GlobalInclude.h"

#ifdef D3D11

// D3D11 inlcude
#include <D3D11.h>

// Windows include
#include <windows.h>

// Engine include
#include "D3DRenderer.h"
#include "Render\Texture.h"

#pragma comment (lib, "D3D11")

namespace DE
{

/*
*	CLASS: D3D11Renderer
*	D3D11Renderer is the interface to the D3D11 rendering device
*	context and swap chain. It controls the binding and unbinding 
*	to rendering device, and contains the actual render target 
*	and G Buffer
*/
class D3D11Renderer : public D3DRenderer
{

public:

	/********************************************************************************
	*	--- Constructor:
	*	D3D11Renderer()
	*	This constructor will do nothing, construction is deferred to construct
	*	function
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	D3D11Renderer() {};

	/********************************************************************************
	*	--- Constructor:
	*	ConstructWithWindow(HWND)
	*	This function will create D3D11 device, context, swap chain, all render target
	*	gbuffer's texture and default states
	*
	*	--- Parameters:
	*	@ hWnd: the window instance
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	bool ConstructWithWindow(HWND hWnd);

	/********************************************************************************
	*	--- Function:
	*	DestructandCleanUp()
	*	This function will release all D3D11 COM object
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void DestructandCleanUp()
	{
		for (int i = 0; i < RT_NUM; i++)
		{
			if (m_pRTVArray[i])
				m_pRTVArray[i]->Release();
		}

		if (m_pSwapChain)
			m_pSwapChain->Release();
		if (m_pD3D11Context)
			m_pD3D11Context->Release();
		if (m_pD3D11Device)
			m_pD3D11Device->Release();
	}

	/********************************************************************************
	*	--- Function:
	*	Update(const float)
	*	This function will update any graphics related class, for example HUD
	*
	*	--- Parameters:
	*	@ delta_time: the delta time of this frame
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Update(const float delta_time);

	/********************************************************************************
	*	--- Function:
	*	Render()
	*	This function will call render from all graphics related class
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Render();

	/********************************************************************************
	*	--- Function:
	*	UnbindRenderTargets()
	*	This function will unbind all render targets from GPU
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void UnbindRenderTargets()
	{
		m_pD3D11Context->OMSetRenderTargets(0, nullptr, nullptr);
	}

	/********************************************************************************
	*	--- Function:
	*	UnBindStreamOutTargets()
	*	This function will unbind a stream out target from GPU
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void UnBindStreamOutTargets()
	{
		ID3D11Buffer* null[1] = { nullptr };
		const unsigned int offset = 0;
		m_pD3D11Context->SOSetTargets(1, null, &offset);
	}

	/********************************************************************************
	*	--- Function:
	*	UnbindPSShaderResources(int)
	*	This function will unbind the given number of shader resources from pixel
	*	shader
	*
	*	--- Parameters:
	*	@ num: the number of pixel shader shader resources to be unbinded
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void UnbindPSShaderResources(int num)
	{
		ID3D11ShaderResourceView* null[5] = { nullptr };
		m_pD3D11Context->PSSetShaderResources(0, num, null);
	}


	ID3D11DeviceContext*						m_pD3D11Context;	// Pointer to interface, handles GPU and pipeline
	ID3D11Device*								m_pD3D11Device;	// Pointer to device, handles video memory
	IDXGISwapChain*								m_pSwapChain;	// Pointer to the swap chain
	ID3D11RenderTargetView*						m_pRTVArray[RT_NUM];	// Array of pointer to cached gbuffer's render target view, cached from m_hTextures
	ID3D11ShaderResourceView*					m_pSRVArray[RT_NUM + 1];	// Array of pointer to cached gbuffer's shader resources view, cached from m_hTextures
	Handle										m_hTextures[RT_NUM];	// Array of pointer to texture for gbuffer
	Texture*									m_backbuffer; // pointer to back buffer texture
	Texture*									m_depth; // pointer to depth texutre for depth stencil view and shader resources view
	Texture*									m_depthReadOnly; // pointer to read-only depth stencil view
	//Texture*									m_LightingAccumBuffer;
};

};
#endif

#endif
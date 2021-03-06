// D3D12Renderer.h: the class for Direct3D 12 renderer
#ifndef D3D12RENDERER_H_
#define D3D12RENDERER_H_

// D3D12 inlcude
#include <d3d12.h>
#include <dxgi1_4.h>
#include "d3dx12.h"

// Windows include
#include <windows.h>

// Engine include
#include "GlobalInclude.h"
#include "RendererBase.h"
#include "Graphics\GPU\GPUCircularAllocator.h"

#ifdef D3D12

#pragma comment (lib, "D3d12") // TODO: move to premake

namespace DE
{

// Engine include
class Texture;

/*
*	CLASS: D3D12Renderer
*	D3D12Renderer is the interface to the D3D12 rendering device
*	context and swap chain. It controls the binding and unbinding
*	to rendering device, and contains the actual render target
*	and G Buffer
*/
class D3D12Renderer : public RendererBase
{

public:

	CUSTOM_MEMORY_DEFINE();

	/********************************************************************************
	*	--- Constructor:
	*	D3D12Renderer()
	*	This constructor will do nothing, construction is deferred to construct
	*	function
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	D3D12Renderer() {};

	/********************************************************************************
	*	--- Constructor:
	*	ConstructWithWindow(HWND)
	*	This function will create D3D12 device, context, swap chain, all render target
	*	gbuffer's texture and default states
	*
	*	--- Parameters:
	*	@ hWnd: the window instance
	*
	*	--- Return:
	*	@ bool: True if succeed, false if failed
	********************************************************************************/
	bool ConstructWithWindow(HWND hWnd);

	/********************************************************************************
	*	--- Function:
	*	DestructandCleanUp()
	*	Empty destructor
	********************************************************************************/
	void DestructandCleanUp()
	{
		// TODO: free resources
	}

	/********************************************************************************
	*	--- Static Function:
	*	GetInstance()
	*	This function will return the singleton instance of D3D12Renderer
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ Renderer*: the singleton instance
	********************************************************************************/
	static D3D12Renderer* GetInstance();

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
	void Update(const float deltaTime);

	/********************************************************************************
	*	--- Function:
	*	UpdatePipeline()
	*	This function will reset command allocator and command list
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void UpdatePipeline();

	void WaitForPreviousFrame();

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

	void ResetCommandAllocatorAndList();

	Texture** GetCurrentBackBufferTextureAddress()
	{
		return &m_backbuffer[m_iCurrFrameIndex];
	}

	const static unsigned int					BACKBUFFER_COUNT = 2;		// Number of frame, single/double/triple buffering

	Texture*									m_backbuffer[BACKBUFFER_COUNT]; // pointer to back buffer texture
	Handle										m_depth; // pointer to depth texutre for depth stencil view and shader resources view
	Texture*									m_depthReadOnly; // pointer to read-only depth stencil view

	ID3D12Debug*								m_pDebugController;
	D3D12_VIEWPORT								m_pViewport;
	D3D12_RECT									m_pScissorRect;
	IDXGISwapChain3*							m_pSwapChain;
	ID3D12Device*								m_pDevice;
	Texture*									m_pRTV[RT_NUM];
	ID3D12DescriptorHeap*						m_pRTVHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE				m_RTVHeapHandle;
	ID3D12DescriptorHeap*						m_pDSVHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE				m_DSVHeapHandle;
	ID3D12DescriptorHeap*						m_pCbvSrvUavHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE				m_CbvSrvUavHeapHandle;
	ID3D12DescriptorHeap*						m_pCbvSrvUavHeapForShader;
	CD3DX12_CPU_DESCRIPTOR_HANDLE				m_CbvSrvUavHeapForShaderHandle;
	ID3D12DescriptorHeap*						m_pSamplerHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE				m_SamplerHeapHandle;
	ID3D12Resource*								m_pDSV;
	ID3D12CommandAllocator*						m_pCommandAllocator[BACKBUFFER_COUNT];
	ID3D12CommandQueue*							m_pCommandQueue;
	ID3D12RootSignature*						m_pRootSignature;
	ID3D12GraphicsCommandList*					m_pCommandList;
	ID3D12Fence*								m_pFence[BACKBUFFER_COUNT];
	HANDLE										m_pFenceEvent;
	UINT										m_iFenceValue[BACKBUFFER_COUNT];
	UINT										m_iRTVDescriptorSize;
	UINT										m_iCurrFrameIndex;
	Handle										m_hTextures[RT_NUM];

	ID3D12Resource*								m_pConstantBufferHeap;
	GPUCircularAllocator*						m_pConstantBufferAllocator;

private:
	static Handle								m_hInstance;		// Singleton instance

};

using Renderer = D3D12Renderer;

};
#endif

#endif
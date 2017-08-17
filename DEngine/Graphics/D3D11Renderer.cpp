// D3D11Renderer.cpp

// Engine include
#include "D3D11Renderer.h"
#include "Scene\SceneGraph.h"
#include "Graphics\Render\State.h"
#include "Graphics\HUD\HUD.h"
#include "Graphics\ParticleSystem\Emitter.h"
#include "Graphics\ParticleSystem\ParticleSystem.h"
#include "DebugRenderer\DEBUG_RENDERER.h"
#include "GlobalInclude.h"
#include "GBuffer.h"
#include "PostProcessEffect.h"

#include <DXGI.h>

#ifdef D3D11

namespace DE
{

bool D3D11Renderer::ConstructWithWindow(HWND hWnd)
{
	IDXGIAdapter * pAdapter;
	MyArray<IDXGIAdapter*> vAdapters(0);
	IDXGIFactory* pFactory = NULL;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);

	DXGI_ADAPTER_DESC adapterDesc;
	size_t maxVideoMemory = 0;
	unsigned int targetAdapterId;
	// pick the adapter with largest dedicated video memory
	for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		vAdapters.Add(pAdapter);
		vAdapters[i]->GetDesc(&adapterDesc);
		if (adapterDesc.DedicatedVideoMemory >= maxVideoMemory)
		{
			maxVideoMemory = adapterDesc.DedicatedVideoMemory;
			targetAdapterId = i;
		}
	}
	const unsigned int size = vAdapters.Size();
	for (int i = 0; i < size; ++i)
	{
		if (i != targetAdapterId)
		{
			vAdapters[i]->Release();
		}
	}

	// Create device and swap chain
	DXGI_SWAP_CHAIN_DESC scData;
	ZeroMemory(&scData, sizeof(scData)); // initialize the whole struct to zero

	scData.BufferCount = 1;
	scData.BufferDesc.Width = WINDOW_WIDTH;
	scData.BufferDesc.Height = WINDOW_HEIGHT;
	scData.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scData.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scData.BufferDesc.RefreshRate.Numerator = 30;
	scData.BufferDesc.RefreshRate.Denominator = 1;
	scData.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	scData.OutputWindow = hWnd;
	scData.SampleDesc.Count = 1; // MSAA
	scData.Windowed = TRUE;
	scData.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scData.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// use default adapter
	HRESULT hr = D3D11CreateDevice(vAdapters[targetAdapterId], D3D_DRIVER_TYPE_UNKNOWN, 0, D3D11_CREATE_DEVICE_DEBUG, NULL, 0,
		D3D11_SDK_VERSION, &m_pD3D11Device, NULL, &m_pD3D11Context);
	assert(hr == S_OK);
	hr = pFactory->CreateSwapChain(m_pD3D11Device, &scData, &m_pSwapChain);
	assert(hr == S_OK);

	if (pFactory)
	{
		pFactory->Release();
	}

	// Create render target view
	for (int i = 0; i < RT_NUM; i++)
	{
		Handle hTexture(sizeof(Texture));
		new (hTexture) Texture(Texture::RENDER_TARGET | Texture::SHADER_RESOURCES);
		m_hTextures[i] = hTexture;
		m_pRTVArray[i] = ((Texture*)m_hTextures[i].Raw())->GetRTV();
		m_pSRVArray[i] = ((Texture*)m_hTextures[i].Raw())->GetSRV();
	}

	ID3D11Texture2D* pBackBufferTex;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBufferTex);
	assert(hr == S_OK);
	Handle hBackBufferTex(sizeof(Texture));
	new (hBackBufferTex) Texture(Texture::RENDER_TARGET | Texture::SHADER_RESOURCES, pBackBufferTex);
	m_backbuffer = ((Texture*)hBackBufferTex.Raw());
	Handle hdepthTex(sizeof(Texture));
	new (hdepthTex) Texture(Texture::DEPTH_STENCIL | Texture::SHADER_RESOURCES);
	m_depth = ((Texture*)hdepthTex.Raw());
	Handle hdepthReadTex(sizeof(Texture));
	new (hdepthReadTex) Texture(Texture::DEPTH_STENCIL_READ_ONLY, m_depth->GetTexture2D());
	m_depthReadOnly = ((Texture*)hdepthReadTex.Raw());
	m_pSRVArray[2] = m_depth->GetSRV();

	//	Handle hLightAccumTex(sizeof(Texture));
	//	new (hLightAccumTex) Texture(Texture::RENDER_TARGET | Texture::SHADER_RESOURCES);
	//	m_LightingAccumBuffer = ((Texture*) hLightAccumTex.Raw());

	// Set viewport
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Height = WINDOW_HEIGHT;
	vp.Width = WINDOW_WIDTH;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pD3D11Context->RSSetViewports(1, &vp);

	State::ConstructDefaultStates(m_pD3D11Device);

	m_GBuffer = new GBuffer;
	m_PPE = new PostProcessEffect;

	return true;
}

void D3D11Renderer::Update(const float delta_time)
{
	//HUD::getInstance()->update(delta_time);
}

void D3D11Renderer::Render()
{
	assert(m_RendererCamera != nullptr);

	// Cleaning screen
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < RT_NUM; i++)
	{
		m_pD3D11Context->ClearRenderTargetView(m_pRTVArray[i], ClearColor);
	}
	m_pD3D11Context->ClearRenderTargetView(m_backbuffer->GetRTV(), ClearColor);
	m_pD3D11Context->ClearDepthStencilView(m_depth->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//	m_pD3D11Context->ClearRenderTargetView(m_LightingAccumBuffer->GetRTV(), ClearColor);

	// Render to texture
	SceneGraph::GetInstance()->FrustumCulling(m_RendererCamera->GetFrustum());
	SceneGraph::GetInstance()->Render();
	SceneGraph::GetInstance()->ShadowMapGeneration();

	// Render to screen
	m_GBuffer->Render();

	// Post process effect
	m_PPE->Render();

	// Particle system drawing
	ParticleSystem::GetInstance()->Render();

	// HUD drawing
//		HUD::getInstance()->Render();

	// Debug drawing
	DEBUG_RENDERER::GetInstance()->Render();

	HRESULT hr = m_pSwapChain->Present(0, 0);

	assert(hr == S_OK);
}

};

#endif
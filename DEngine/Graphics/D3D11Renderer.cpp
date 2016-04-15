// D3D11Renderer.cpp

// Engine include
#include "D3D11Renderer.h"
#include "Scene\SceneGraph.h"
#include "Graphics\Render\State.h"
#include "Graphics\HUD\HUD.h"
#include "Graphics\ParticleSystem\Emitter.h"
#include "Graphics\ParticleSystem\ParticleSystem.h"
#include "DebugRenderer\DEBUG_RENDERER.h"

#include <vector>
#include <DXGI.h>

namespace DE
{

	D3D11Renderer* D3D11Renderer::m_pInstance = nullptr;

	void D3D11Renderer::ConstructWithWindow(HWND hWnd)
	{
		IDXGIAdapter * pAdapter;
		std::vector <IDXGIAdapter*> vAdapters;
		IDXGIFactory* pFactory = NULL;
		CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);

		for (UINT i = 0;
		pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND;
			++i)
		{
			vAdapters.push_back(pAdapter);
		}

		DXGI_ADAPTER_DESC adapterDesc;
		for (IDXGIAdapter* itr : vAdapters)
		{
			itr->GetDesc(&adapterDesc);
		}

		static wchar_t s[64];
		swprintf(s, 64, L"Adpaters: %i\n", vAdapters.size());
		OutputDebugStringW(s);

		// Create device and swap chain
		DXGI_SWAP_CHAIN_DESC scData;
		ZeroMemory(&scData, sizeof(scData)); // initialize the whole struct to zero

		scData.BufferCount = 1;
		scData.BufferDesc.Width = 1024;
		scData.BufferDesc.Height = 768;
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

		HRESULT hr = D3D11CreateDevice(vAdapters[1], D3D_DRIVER_TYPE_UNKNOWN, 0, D3D11_CREATE_DEVICE_DEBUG, NULL, 0,
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
		vp.Height = 768.0f;
		vp.Width = 1024.0f;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		m_pD3D11Context->RSSetViewports(1, &vp);

		State::ConstructDefaultStates(m_pD3D11Device);

		m_GBuffer = new GBuffer;
		m_PPE = new PostProcessEffect;
	}

	void D3D11Renderer::Update(const float delta_time, const float curr_time)
	{
		HUD::getInstance()->update(delta_time);
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
//		m_PPE->Render();

		// Particle system drawing
		ParticleSystem::GetInstance()->Render();

		// HUD drawing
		HUD::getInstance()->Render();

		// Debug drawing
		DEBUG_RENDERER::GetInstance()->Render();

		HRESULT hr = m_pSwapChain->Present(0, 0);

		assert(hr == S_OK);
	}

};
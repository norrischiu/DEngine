// D3D11Renderer.cpp

// Engine include
#include "D3D11Renderer.h"
#include "Scene\SceneGraph.h"
#include "Graphics\Render\State.h"
#include "Graphics\HUD\HUD.h"
#include "Graphics\ParticleSystem\Emitter.h"
#include "Graphics\ParticleSystem\ParticleSystem.h"

namespace DE
{

D3D11Renderer* D3D11Renderer::m_pInstance = nullptr;
void D3D11Renderer::ConstructWithWindow(HWND hWnd)
{
	// Create device and swap chain
	DXGI_SWAP_CHAIN_DESC scData;
	ZeroMemory(&scData, sizeof(scData)); // initialize the whole struct to zero

	scData.BufferCount = 1;
	scData.BufferDesc.Width = 1024;
	scData.BufferDesc.Height = 768;
	scData.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scData.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scData.BufferDesc.RefreshRate.Numerator = 60;
	scData.BufferDesc.RefreshRate.Denominator = 1;
	scData.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scData.OutputWindow = hWnd;
	scData.SampleDesc.Count = 1; // MSAA
	scData.Windowed = TRUE;
	scData.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scData.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0,
		D3D11_SDK_VERSION, &scData, &m_pSwapChain, &m_pD3D11Device, NULL, &m_pD3D11Context);
	assert(hr == S_OK);

	// Create render target view
	for (int i = 0; i < RT_NUM; i++)
	{
		m_textures[i] = new Texture(Texture::RENDER_TARGET | Texture::SHADER_RESOURCES);
		m_pRTVArray[i] = m_textures[i]->GetRTV();
		m_pSRVArray[i] = m_textures[i]->GetSRV();
	}

	ID3D11Texture2D* pBackBufferTex;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBufferTex);
	assert(hr == S_OK);
	m_backbuffer = new Texture(Texture::RENDER_TARGET, pBackBufferTex);
	m_depth = new Texture(Texture::DEPTH_STENCIL | Texture::SHADER_RESOURCES);
	m_depthReadOnly = new Texture(Texture::DEPTH_STENCIL_READ_ONLY, m_depth->GetTexture2D());
	m_pSRVArray[2] = m_depth->GetSRV();

	m_LightingAccumBuffer = new Texture(Texture::RENDER_TARGET | Texture::SHADER_RESOURCES);

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

//	Emitter::GetInstance()->AddParticle(1, Vector3(2.5f, 0.5f, 0.0f), Vector3(0.0f, 0.7f, 0.0f));
	// adding particles: input variables(particles id, particles type(torch flame), emitting position, emitting direction)
	//ParticleSystem::GetInstance()->AddParticles("torch_flame_1", 1, Vector3(-2.5f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	//ParticleSystem::GetInstance()->AddParticles("smoke", 2, Vector3(0.0f, 1.5f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	//ParticleSystem::GetInstance()->AddParticles("fire", 4, Vector3(2.0f, 1.5f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
}

void D3D11Renderer::Update(const float delta_time, const float curr_time)
{
//	HUD::getInstance()->update(delta_time);
//	Emitter::GetInstance()->Update(delta_time);
	//ParticleSystem::GetInstance()->Update(delta_time);
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
	m_pD3D11Context->ClearRenderTargetView(m_LightingAccumBuffer->GetRTV(), ClearColor);

	// Render to texture
	SceneGraph::GetInstance()->FrustumCulling(m_RendererCamera->GetFrustum());
	SceneGraph::GetInstance()->Render();
	SceneGraph::GetInstance()->ShadowMapGeneration();

	// Render to screen
	m_GBuffer->Render();

	// Post process effect
//	m_PPE->Render();

	// Particle system drawing
//	Emitter::GetInstance()->Draw();
	ParticleSystem::GetInstance()->Render();

	// HUD drawing
//	HUD::getInstance()->Render();

	// Debug drawing
	SceneGraph::GetInstance()->RENDER_DEBUG_DRAWING();

	HRESULT hr = m_pSwapChain->Present(0, 0);
	assert(hr == S_OK);
}

};
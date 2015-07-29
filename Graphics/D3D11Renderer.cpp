// D3D11Renderer.cpp

#include "D3D11Renderer.h"

D3D11Renderer* D3D11Renderer::m_pInstance;

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
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer); // needs assert

	hr = m_pD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView); // get buffer through 2D texture
	assert(hr == S_OK);
	pBackBuffer->Release();

	// Set rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;

	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState);
	assert(hr == S_OK);
	D3D11Renderer::getInstance()->m_pD3D11Context->RSSetState(m_pRasterizerState);

	// Set the depth state
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = 1024;
	depthStencilDesc.Height = 768;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;

	m_pD3D11Device->CreateTexture2D(
		&depthStencilDesc, // Description of texture to create.
		0, &
		mDepthStencilBuffer);

	m_pD3D11Device->CreateDepthStencilView(mDepthStencilBuffer, // Resource we want to create a view to.
		0,
		&mDepthStencilView);

	// Set viewport
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Height = 768;
	vp.Width = 1024;
	vp.MaxDepth = 1.0f;
	m_pD3D11Context->RSSetViewports(1, &vp);

	// temp testing
	m_MeshInstaceList.push_back(new MeshInstance(NULL));
}

void D3D11Renderer::Update()
{
}

void D3D11Renderer::Render()
{
	// Cleaning screen
	float ClearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_pD3D11Context->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

	m_pD3D11Context->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	for (auto itr = m_MeshInstaceList.begin(); itr != m_MeshInstaceList.end(); ++itr)
		(*itr)->Draw();

	HRESULT hr = m_pSwapChain->Present(0, 0);
	assert(hr == S_OK);
}
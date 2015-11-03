// DeferredRenderer.cpp

#include "DeferredRenderer.h"
#include "Scene\RootSceneNode.h"

DeferredRenderer* DeferredRenderer::m_pInstance;

void DeferredRenderer::ConstructWithWindow(HWND hWnd)
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
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	hr = m_pD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView[0]); // get buffer through 2D texture
	assert(hr == S_OK);

	// Set the depth resource
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

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState * pDSState;
	m_pD3D11Device->CreateDepthStencilState(&dsDesc, &pDSState);
	m_pD3D11Context->OMSetDepthStencilState(pDSState, 1);

	/*
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	*/

	ID3D11Texture2D* pDepthStencilBuffer;
	m_pD3D11Device->CreateTexture2D(&depthStencilDesc, 0, &pDepthStencilBuffer);
	hr = m_pD3D11Device->CreateDepthStencilView(pDepthStencilBuffer, NULL, &m_pDepthStencilView);
	assert(hr == S_OK);

	// Set rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;

	ID3D11RasterizerState* pRasterizerState;
	hr = m_pD3D11Device->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
	assert(hr == S_OK);
	m_pD3D11Context->RSSetState(pRasterizerState);

	// Set viewport
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Height = 768;
	vp.Width = 1024;
	vp.MaxDepth = 1.0f;
	m_pD3D11Context->RSSetViewports(1, &vp);

	if (pDepthStencilBuffer)
		pDepthStencilBuffer->Release();
	if (pBackBuffer)
		pBackBuffer->Release();
	if (pRasterizerState)
		pRasterizerState->Release();
}

void DeferredRenderer::Render()
{
	// Cleaning screen
	float ClearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_pD3D11Context->ClearRenderTargetView(m_pRenderTargetView[0], ClearColor);
	m_pD3D11Context->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_pD3D11Context->OMSetRenderTargets(1, &m_pRenderTargetView[0], m_pDepthStencilView);

	//for (auto itr = m_MeshComponentList.begin(); itr != m_MeshComponentList.end(); ++itr)
	//	(*itr)->Draw();

	RootSceneNode::getInstance()->RenderSubNodes();

	HRESULT hr = m_pSwapChain->Present(0, 0);
	assert(hr == S_OK);
}
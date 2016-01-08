// D3D11Renderer.cpp

#include "D3D11Renderer.h"
#include "Scene\SceneGraph.h"

D3D11Renderer* D3D11Renderer::m_pInstance;

D3D11Renderer::D3D11Renderer() : m_currrent_camera_type(CameraType::MOVE_CAMERA)
{
	const float EPS = std::numeric_limits<float>::epsilon();
	m_camera = new CameraComponent[8]{
		{ Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.7f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },	//MOVE
		{ Vector3(0.0f, 0.0f, 15.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },		//DEGREE_360
		{ Vector3(0.0f, 15.0f, EPS), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },	//TOP
		{ Vector3(0.0f, -15.0f, EPS), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },	//BOTTOM
		{ Vector3(15.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },	//LEFT
		{ Vector3(-15.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },	//RIGHT
		{ Vector3(0.0f, 0.0f, 15.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },		//FRONT
		{ Vector3(0.0f, 0.0f, -15.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) }		//BACK
	};
}

D3D11Renderer* D3D11Renderer::GetInstance() {
	if (!m_pInstance)
		m_pInstance = new D3D11Renderer();
	return m_pInstance;
}

CameraComponent* D3D11Renderer::GetCamera() {
	//return &m_camera[m_currrent_camera_type];
	return m_renderCamera;
}

CameraType D3D11Renderer::GetCameraType() {
	return m_currrent_camera_type;
}

void D3D11Renderer::SetCamera(CameraType cameraType) {
	if (cameraType >= CameraType::MOVE_CAMERA && cameraType <= CameraType::BACK_VIEW_CAMERA) {
		m_currrent_camera_type = cameraType;
	}
}

void D3D11Renderer::SetCamera(CameraComponent* camera)
{
	m_renderCamera = camera;
}

std::vector<MeshComponent*>& D3D11Renderer::GetMeshComponentList() {
	return m_MeshComponentList;
}

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

	// Setup the render target texture description.
	D3D11_TEXTURE2D_DESC RTTextureDesc;
	RTTextureDesc.Width = 1024;
	RTTextureDesc.Height = 768;
	RTTextureDesc.MipLevels = 1;
	RTTextureDesc.ArraySize = 1;
	RTTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	RTTextureDesc.SampleDesc.Count = 1;
	RTTextureDesc.SampleDesc.Quality = 0;
	RTTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	RTTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	RTTextureDesc.CPUAccessFlags = 0;
	RTTextureDesc.MiscFlags = 0;

	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = RTTextureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Setup the description of the shader resource view.
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = RTTextureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create render target view
	for (int i = 0; i < RT_NUM; i++)
	{
		hr = m_pD3D11Device->CreateTexture2D(&RTTextureDesc, 0, &m_pTexture[i]);
		assert(hr == S_OK);
		hr = m_pD3D11Device->CreateRenderTargetView(m_pTexture[i], &renderTargetViewDesc, &m_pRenderTargetView[i]); // get buffer through 2D texture
		assert(hr == S_OK);
		hr = m_pD3D11Device->CreateShaderResourceView(m_pTexture[i], &shaderResourceViewDesc, &m_pShaderResourceView[i]);
		assert(hr == S_OK);
	}
	ID3D11Texture2D* pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	hr = m_pD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBufferRTView);

	// Set the depth resource
	D3D11_TEXTURE2D_DESC depthStencilTexDesc;
	depthStencilTexDesc.Width = 1024;
	depthStencilTexDesc.Height = 768;
	depthStencilTexDesc.MipLevels = 1;
	depthStencilTexDesc.ArraySize = 1;
	depthStencilTexDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilTexDesc.SampleDesc.Count = 1;
	depthStencilTexDesc.SampleDesc.Quality = 0;
	depthStencilTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilTexDesc.CPUAccessFlags = 0;
	depthStencilTexDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Depth test parameters
	depthStencilDesc.DepthEnable = true; // true
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0;
	depthStencilDesc.StencilWriteMask = 0;

	// Create depth stencil state
	m_pD3D11Device->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = 0x00;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	hr = m_pD3D11Device->CreateDepthStencilState(&depthStencilDesc, &m_pOffDepthStencilState);
	assert(hr == S_OK);
	depthStencilDesc.StencilEnable = false;
	hr = m_pD3D11Device->CreateDepthStencilState(&depthStencilDesc, &m_pOffDepthOffStencilState);
	assert(hr == S_OK);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	depthStencilViewDesc.Flags = 0;
	
	ID3D11Texture2D* pDepthStencilBuffer;
	hr = m_pD3D11Device->CreateTexture2D(&depthStencilTexDesc, 0, &pDepthStencilBuffer);
	assert(hr == S_OK);
	hr = m_pD3D11Device->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	depthStencilViewDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;
	hr = m_pD3D11Device->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilReadOnlyView);
	assert(hr == S_OK);
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	hr = m_pD3D11Device->CreateShaderResourceView(pDepthStencilBuffer, &shaderResourceViewDesc, &m_pShaderResourceView[2]);
	assert(hr == S_OK);

	// Set rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;

	hr = m_pD3D11Device->CreateRasterizerState(&rasterizerDesc, &m_pCullBackRSState);
	assert(hr == S_OK);
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	hr = m_pD3D11Device->CreateRasterizerState(&rasterizerDesc, &m_pCullNoneRSState);
	assert(hr == S_OK);
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = m_pD3D11Device->CreateRasterizerState(&rasterizerDesc, &m_pFillWireRSState);
	assert(hr == S_OK);

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

	if (pDepthStencilBuffer)
		pDepthStencilBuffer->Release();
	if (pBackBuffer)
		pBackBuffer->Release();

	m_GBuffer = new GBuffer;

	D3D11_BLEND_DESC blendStateDesc;
	blendStateDesc.AlphaToCoverageEnable = TRUE;
	blendStateDesc.IndependentBlendEnable = FALSE;
	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_pD3D11Device->CreateBlendState(&blendStateDesc, &m_pBlendState);
	assert(hr == S_OK);
}

void D3D11Renderer::Update()
{
}

void D3D11Renderer::Render()
{
	assert(m_renderCamera != nullptr);

	// Cleaning screen
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < RT_NUM; i++)
	{
		m_pD3D11Context->ClearRenderTargetView(m_pRenderTargetView[i], ClearColor);
	}
	m_pD3D11Context->ClearRenderTargetView(m_pBackBufferRTView, ClearColor);
	m_pD3D11Context->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pD3D11Context->OMSetDepthStencilState(m_pDepthStencilState, 1);
	m_pD3D11Context->RSSetState(m_pCullBackRSState);
	m_pD3D11Context->OMSetBlendState(nullptr, NULL, 0xFFFFFF);

	// Render to texture
	m_pD3D11Context->OMSetRenderTargets(RT_NUM-1, m_pRenderTargetView, m_pDepthStencilView);
	SceneGraph::GetInstance()->Render();
	ID3D11ShaderResourceView* null[] = { nullptr, nullptr };
	m_pD3D11Context->PSSetShaderResources(0, 2, null);

	// Render to screen
	m_pD3D11Context->OMSetRenderTargets(0, 0, m_pDepthStencilView);
	m_GBuffer->Render();

#ifdef _DEBUG
	SceneGraph::GetInstance()->RENDER_DEBUG_DRAWING();
#endif

	HRESULT hr = m_pSwapChain->Present(0, 0);
	assert(hr == S_OK);
}
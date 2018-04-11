#include "D3D12Renderer.h"
#include "Graphics\Scene\SceneGraph.h"
#include "Graphics\Render\State.h"
#include "Graphics\GBuffer.h"

namespace DE 
{

Handle D3D12Renderer::m_hInstance;

D3D12Renderer* D3D12Renderer::GetInstance()
{
	if (m_hInstance.Raw() == nullptr)
	{
		m_hInstance.Set(sizeof(D3D12Renderer));
		new (m_hInstance) D3D12Renderer();
	}
	return reinterpret_cast<D3D12Renderer*>(m_hInstance.Raw());
}

bool D3D12Renderer::ConstructWithWindow(HWND hWnd)
{
	HRESULT hr;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebugController))))
	{
		m_pDebugController->EnableDebugLayer();
	}

	IDXGIFactory4* dxgiFactory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr))
	{
		return false;
	}
	IDXGIAdapter3* adapter;
	int adapterIndex = 0;

	// Device
	bool hasAdapter = false;
	while (dxgiFactory->EnumAdapters(adapterIndex, reinterpret_cast<IDXGIAdapter**>(&adapter)) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			adapterIndex++;
			continue;
		}
		hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(hr))
		{
			hasAdapter = true;
			break;
		}
		adapterIndex++;
	}
	if (!hasAdapter)
	{
		return false;
	}

	// Query memory
	{
		DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
		adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);
		size_t vram = videoMemoryInfo.Budget;
	}

	// Create the device
	hr = D3D12CreateDevice(
		adapter,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_pDevice)
		);
	if (FAILED(hr))
	{
		return false;
	}

	// Command allocator, queue, list
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hr = m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));

	for (int i = 0; i < BACKBUFFER_COUNT; ++i)
	{
		hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator[i]));
		assert(hr == S_OK);
	}

	hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocator[0], nullptr, IID_PPV_ARGS(&m_pCommandList));
	assert(hr == S_OK);

	// Swap chain.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = WINDOW_WIDTH;
	swapChainDesc.BufferDesc.Height = WINDOW_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;
	IDXGISwapChain* swapChain;
	hr = dxgiFactory->CreateSwapChain(m_pCommandQueue, &swapChainDesc, &swapChain);
	assert(hr == S_OK);
	m_pSwapChain = static_cast<IDXGISwapChain3*>(swapChain);
	m_iCurrFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
	
	// create upload heap for constant buffer
	{
		m_pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, // no flags
			&CD3DX12_RESOURCE_DESC::Buffer(32 * 1024 * 1024),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_pConstantBufferHeap));
		m_pConstantBufferHeap->SetName(L"Constant Buffer Upload Resource Heap");

		m_pConstantBufferAllocator = new GPUCircularAllocator(m_pConstantBufferHeap, 32 * 1024 * 1024);
	}

	// RTV descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC RTVheapDesc = {};
	RTVheapDesc.NumDescriptors = 16;
	RTVheapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVheapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = m_pDevice->CreateDescriptorHeap(&RTVheapDesc, IID_PPV_ARGS(&m_pRTVHeap));
	assert(hr == S_OK);
	m_RTVHeapHandle = m_pRTVHeap->GetCPUDescriptorHandleForHeapStart();

	// DSV descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC DSVHeapDesc = {};
	DSVHeapDesc.NumDescriptors = 16;
	DSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = m_pDevice->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(&m_pDSVHeap));
	assert(hr == S_OK);
	m_DSVHeapHandle = m_pDSVHeap->GetCPUDescriptorHandleForHeapStart();

	// CBV and SRV descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1024;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = m_pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pCbvSrvUavHeap));
	assert(hr == S_OK);
	m_CbvSrvUavHeapHandle = m_pCbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart();

	// Sampler descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
	samplerHeapDesc.NumDescriptors = 64;
	samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = m_pDevice->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_pSamplerHeap));
	assert(hr == S_OK);
	m_SamplerHeapHandle = m_pSamplerHeap->GetCPUDescriptorHandleForHeapStart();

	// early creation of cbuffer for now, so CBV is at the top of the heap
	State::ConstructDefaultStates();
	SceneGraph::GetInstance();
	m_GBuffer = new GBuffer;

	m_SrvHeapOffset = (m_CbvSrvUavHeapHandle.ptr - m_pCbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart().ptr) / m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int i = 0; i < BACKBUFFER_COUNT; ++i)
	{
		ID3D12Resource* pBackBufferTex;
		hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBufferTex));
		assert(hr == S_OK);
		Handle hBackBufferTex(sizeof(Texture));
		new (hBackBufferTex) Texture(Texture::RENDER_TARGET, pBackBufferTex);
		m_backbuffer[i] = ((Texture*)hBackBufferTex.Raw());
		m_backbuffer[i]->GetTexture2D()->SetName(L"Back Buffer");
	}

	// Render target view
	for (int i = 0; i < RT_NUM; i++)
	{
		Handle hTexture(sizeof(Texture));
		new (hTexture) Texture(Texture::RENDER_TARGET | Texture::SHADER_RESOURCES);
		m_hTextures[i] = hTexture;
		m_pRTV[i] = (Texture*)m_hTextures[i].Raw();
		char str[16];
		snprintf(str, sizeof(str), "GBuffer RT%i", i);
		wchar_t name[16];
		mbstowcs(name, str, strlen(str) + 1);
		m_pRTV[i]->GetTexture2D()->SetName(name);
	}

	Handle hdepthTex(sizeof(Texture));
	new (hdepthTex) Texture(Texture::DEPTH_STENCIL | Texture::SHADER_RESOURCES);
	m_depth = ((Texture*)hdepthTex.Raw());
	m_depth->GetTexture2D()->SetName(L"Depth Buffer");
	Handle hdepthReadTex(sizeof(Texture));
	new (hdepthReadTex) Texture(Texture::DEPTH_STENCIL_READ_ONLY, m_depth->GetTexture2D());
	m_depthReadOnly = ((Texture*)hdepthReadTex.Raw());
	m_depthReadOnly->GetTexture2D()->SetName(L"Depth Buffer read only");

	// TODO: create a null texture? or smart root paramater range 

	// Create fence and fence event
	for (int i = 0; i < BACKBUFFER_COUNT; ++i)
	{
		hr = m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence[i]));
		if (FAILED(hr))
		{
			return false;
		}
		m_iFenceValue[i] = 0;
	}
	m_pFenceEvent = CreateEvent(nullptr, false, false, nullptr);

	m_GBuffer->InitializeMeshAndRenderPass();

	// Fill out the Viewport
	m_pViewport.TopLeftX = 0;
	m_pViewport.TopLeftY = 0;
	m_pViewport.Width = WINDOW_WIDTH;
	m_pViewport.Height = WINDOW_HEIGHT;
	m_pViewport.MinDepth = 0.0f;
	m_pViewport.MaxDepth = 1.0f;

	// Fill out a scissor rect
	m_pScissorRect.left = 0;
	m_pScissorRect.top = 0;
	m_pScissorRect.right = WINDOW_WIDTH;
	m_pScissorRect.bottom = WINDOW_HEIGHT;

	return true;
}

void D3D12Renderer::Update(const float deltaTime)
{
}

void D3D12Renderer::UpdatePipeline()
{
	HRESULT hr;

	// Barrier: from Present to RT
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_backbuffer[m_iCurrFrameIndex]->GetTexture2D();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_pCommandList->ResourceBarrier(1, &barrier);

	// Cleaning screen
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < RT_NUM; i++)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(((Texture*)m_hTextures[i].Raw())->GetRTV(), 0);
		m_pCommandList->ClearRenderTargetView(rtvHandle, ClearColor, 0, nullptr);
		//rtvHandle.Offset(1, m_iRTVDescriptorSize);
	}
	m_pCommandList->ClearRenderTargetView(m_backbuffer[m_iCurrFrameIndex]->GetRTV(), ClearColor, 0, nullptr);
	m_pCommandList->ClearDepthStencilView(m_depth->GetDSV(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	m_pCommandList->RSSetViewports(1, &m_pViewport);
	m_pCommandList->RSSetScissorRects(1, &m_pScissorRect);

	// set constant buffer descriptor heap
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_pCbvSrvUavHeap, m_pSamplerHeap };
	m_pCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// Render to texture
	SceneGraph::GetInstance()->FrustumCulling(m_RendererCamera);
	SceneGraph::GetInstance()->Render(this);
	//SceneGraph::GetInstance()->ShadowMapGeneration();

	m_GBuffer->Render(this);
	//m_PPE->Render();
	//ParticleSystem::GetInstance()->Render();
	//DEBUG_RENDERER::GetInstance()->Render();

//	m_pCommandList->OMSetRenderTargets(1, &m_backbuffer[m_iCurrFrameIndex]->GetRTV()->GetCPUDescriptorHandleForHeapStart(), FALSE, nullptr);

	// Barrier: from RT to Present
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_backbuffer[m_iCurrFrameIndex]->GetTexture2D();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	m_pCommandList->ResourceBarrier(1, &barrier);

	hr = m_pCommandList->Close();
	if (FAILED(hr))
	{
		//Running = false;
	}
}

void D3D12Renderer::WaitForPreviousFrame()
{
	HRESULT hr;

	m_iCurrFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	if (m_pFence[m_iCurrFrameIndex]->GetCompletedValue() < m_iFenceValue[m_iCurrFrameIndex])
	{
		hr = m_pFence[m_iCurrFrameIndex]->SetEventOnCompletion(m_iFenceValue[m_iCurrFrameIndex], m_pFenceEvent);
		assert(SUCCEEDED(hr));
		WaitForSingleObject(m_pFenceEvent, INFINITE);
	}

	m_iFenceValue[m_iCurrFrameIndex]++;
}

void D3D12Renderer::Render()
{
	HRESULT hr;
	assert(m_RendererCamera != nullptr);

	UpdatePipeline();

	ID3D12CommandList* ppCommandLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	hr = m_pCommandQueue->Signal(m_pFence[m_iCurrFrameIndex], m_iFenceValue[m_iCurrFrameIndex]);
	hr = m_pSwapChain->Present(0, 0);

	ResetCommandAllocatorAndList();
	m_pConstantBufferAllocator->Reset();
}

void D3D12Renderer::ResetCommandAllocatorAndList()
{
	WaitForPreviousFrame();

	// clear for next frame
	HRESULT hr;
	hr = m_pCommandAllocator[m_iCurrFrameIndex]->Reset();
	assert(SUCCEEDED(hr));
	hr = m_pCommandList->Reset(m_pCommandAllocator[m_iCurrFrameIndex], NULL);
	assert(SUCCEEDED(hr));
}

}
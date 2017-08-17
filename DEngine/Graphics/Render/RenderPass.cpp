#include "RenderPass.h"
#include "Graphics\D3DRenderer.h"
#include "Graphics\D3D11Renderer.h"
#include "Graphics\D3D12Renderer.h"
#include "GlobalInclude.h"

namespace DE
{

#ifdef D3D12
void RenderPass::ConstructPSO()
{
	D3D12Renderer* renderer = (D3D12Renderer*)D3DRenderer::GetInstance();
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	ID3DBlob* signature;
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	HRESULT hr;

	// Vertex shader
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[0].Descriptor.RegisterSpace = 0;
	rootParameters[0].Descriptor.ShaderRegister = 1;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.RegisterSpace = 0;
	rootParameters[1].Descriptor.ShaderRegister = 2;

	// Pixel shader
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].Descriptor.RegisterSpace = 0;
	rootParameters[2].Descriptor.ShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE  PSDescriptorTableRanges[1] = {};
	PSDescriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	PSDescriptorTableRanges[0].NumDescriptors = 3;
	PSDescriptorTableRanges[0].BaseShaderRegister = 0;
	PSDescriptorTableRanges[0].RegisterSpace = 0;
	PSDescriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[3].DescriptorTable.pDescriptorRanges = &PSDescriptorTableRanges[0];
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;

	rootSignatureDesc.NumStaticSamplers = 1;
	rootSignatureDesc.pStaticSamplers = &State::GetState(State::LINEAR_MIPMAP_MAX_LOD_SS).SS;

	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.NumParameters = 4;
	rootSignatureDesc.pParameters = rootParameters;
	hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
	((D3D12Renderer*)D3DRenderer::GetInstance())->m_pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = m_pRootSignature;
	D3D12_SHADER_BYTECODE VS;
	VS.pShaderBytecode = ((ID3DBlob*)m_pVS)->GetBufferPointer();
	VS.BytecodeLength = ((ID3DBlob*)m_pVS)->GetBufferSize();
	psoDesc.VS = VS;
	psoDesc.InputLayout.pInputElementDescs = m_InputLayout;
	psoDesc.InputLayout.NumElements = m_InputLayoutCount;
	if (m_pPS != nullptr)
	{
		D3D12_SHADER_BYTECODE PS;
		PS.pShaderBytecode = ((ID3DBlob*)m_pPS)->GetBufferPointer();
		PS.BytecodeLength = ((ID3DBlob*)m_pPS)->GetBufferSize();
		psoDesc.PS = PS;
	}
	if (m_iRS != 0)
	{
		psoDesc.RasterizerState = State::GetState(m_iRS).RS;
	}
	else
	{
		psoDesc.RasterizerState = State::GetState(State::CULL_NONE_RS).RS;
	}
	if (m_iBS != 0)
	{
		psoDesc.BlendState = State::GetState(m_iBS).BS;
	}
	if (m_iDS != 0)
	{
		psoDesc.DepthStencilState = State::GetState(m_iDS).DS;
		psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}
	psoDesc.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)m_iTopology;
	psoDesc.NumRenderTargets = m_iRTVNum;
	for (int i = 0; i < m_iRTVNum; ++i)
	{
		psoDesc.RTVFormats[i] = m_pRTVs[i]->GetFormat();
	}
	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;
	psoDesc.SampleMask = 0xffffff;
	psoDesc.SampleDesc = sampleDesc;

	hr = ((D3D12Renderer*)D3DRenderer::GetInstance())->m_pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPSO));
	assert(hr == S_OK);
}
#endif

void RenderPass::BindToRenderer()
{
#ifdef D3D12
	D3D12Renderer* renderer = (D3D12Renderer*)D3DRenderer::GetInstance();
	if (m_pPS != nullptr)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE SRVStart = renderer->m_pCbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();
		SRVStart.ptr += m_vTextureSRVs[0].ptr - renderer->m_pCbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart().ptr;
		renderer->m_pCommandList->SetGraphicsRootDescriptorTable(3, SRVStart);
	}
	renderer->m_pCommandList->SetPipelineState(m_pPSO);
#elif defined D3D11
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->IASetInputLayout(m_pInputLayout);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->VSSetShader(m_pVS, 0, 0);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->HSSetShader(m_pHS, 0, 0);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->DSSetShader(m_pDS, 0, 0);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->GSSetShader(m_pGS, 0, 0);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->PSSetShader(m_pPS, 0, 0);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->RSSetState(m_pRS);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->OMSetDepthStencilState(m_pDSS, 1);
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->OMSetBlendState(m_pBS, NULL, 0xFFFFFF);

	if (m_pSOTarget)
	{
		const unsigned int offset = 0;
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->SOSetTargets(1, &m_pSOTarget, &offset);
	}
#endif

	if (m_iRTVNum > 0)
	{
#ifdef D3D12
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[8] = {};
		for (int i = 0; i < m_iRTVNum; ++i)
		{
			rtvHandles[i] = m_pRTVs[i]->GetRTV();
		}

		if (m_pDSV != nullptr)
		{
			((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->OMSetRenderTargets(m_iRTVNum, rtvHandles, FALSE, &m_pDSV->GetDSV()->GetCPUDescriptorHandleForHeapStart());
		}
		else
		{
			((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->OMSetRenderTargets(m_iRTVNum, rtvHandles, FALSE, nullptr);
		}
#elif defined D3D11
		((D3D11Renderer*)D3DRenderer::GetInstance())->UnbindRenderTargets();
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->OMSetRenderTargets(m_iRTVNum, m_pRTVs, m_pDSV);
#endif
	}
	else
	{
#ifdef D3D12
		// already in write depth state when create
		if (m_pDSV != nullptr)
		{
			((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->OMSetRenderTargets(0, nullptr, FALSE, &m_pDSV->GetDSV()->GetCPUDescriptorHandleForHeapStart());
		}
		else
		{
			((D3D12Renderer*)D3DRenderer::GetInstance())->m_pCommandList->OMSetRenderTargets(0, nullptr, FALSE, nullptr);
		}
#elif defined D3D11
		((D3D11Renderer*)D3DRenderer::GetInstance())->UnbindRenderTargets();
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->OMSetRenderTargets(0, nullptr, m_pDSV);
#endif
	}

#ifdef D3D11
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->PSSetShaderResources(0, m_vTextureSRVs.Size(), m_vTextureSRVs.Raw());
	((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->PSSetSamplers(0, m_vSamplerState.Size(), m_vSamplerState.Raw());
	//((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->VSSetShaderResources(i, 1, &m_vTextureSRVs[i]);
	//((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->VSSetSamplers(0, 1, &m_vSamplerState[i]);
	if (m_pDS)
	{
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->DSSetShaderResources(0, m_vTextureSRVs.Size(), m_vTextureSRVs.Raw());
		((D3D11Renderer*)D3DRenderer::GetInstance())->m_pD3D11Context->DSSetSamplers(0, m_vSamplerState.Size(), m_vSamplerState.Raw());
	}
#endif
}

};
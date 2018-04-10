#include "RenderPass.h"
#include "Graphics\D3D12Renderer.h"
#include "GlobalInclude.h"

namespace DE
{

void RenderPass::ConstructPSO()
{
	Renderer* renderer = Renderer::GetInstance();
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
	Renderer::GetInstance()->m_pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));

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

	hr = Renderer::GetInstance()->m_pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPSO));
	assert(hr == S_OK);
}

void RenderPass::BindToRenderer(Renderer* renderer)
{
	if (m_pPS != nullptr)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE SRVStart = renderer->m_pCbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();
		UINT64 diff = m_vTextureSRVs[0].ptr - renderer->m_pCbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart().ptr;
		SRVStart.ptr += diff;
		renderer->m_pCommandList->SetGraphicsRootDescriptorTable(3, SRVStart);
	}
	renderer->m_pCommandList->SetPipelineState(m_pPSO);

	if (m_iRTVNum > 0)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[8] = {};
		for (int i = 0; i < m_iRTVNum; ++i)
		{
			rtvHandles[i] = m_pRTVs[i]->GetRTV();
		}

		if (m_pDSV != nullptr)
		{
			renderer->m_pCommandList->OMSetRenderTargets(m_iRTVNum, rtvHandles, FALSE, &m_pDSV->GetDSV()->GetCPUDescriptorHandleForHeapStart());
		}
		else
		{
			renderer->m_pCommandList->OMSetRenderTargets(m_iRTVNum, rtvHandles, FALSE, nullptr);
		}
	}
	else
	{
		// already in write depth state when create
		if (m_pDSV != nullptr)
		{
			renderer->m_pCommandList->OMSetRenderTargets(0, nullptr, FALSE, &m_pDSV->GetDSV()->GetCPUDescriptorHandleForHeapStart());
		}
		else
		{
			renderer->m_pCommandList->OMSetRenderTargets(0, nullptr, FALSE, nullptr);
		}
	}
}

};
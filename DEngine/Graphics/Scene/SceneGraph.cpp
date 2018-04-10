#include "SceneGraph.h"
#include "Graphics\MeshComponent.h"
#include "Graphics\Animation\Skeleton.h"
#include "Graphics\Animation\AnimationController.h"
#include "Object\CameraComponent.h"
#include "Light\LightManager.h"
#include "Graphics\Render\State.h"
#include "Graphics\Render\RenderPass.h"
#include "Graphics\Render\VSPerObjectCBuffer.h"
#include "Graphics\Render\PSPerMaterialCBuffer.h"
#include "Graphics\Render\VSMatrixPaletteCBuffer.h"
#include "Graphics\Render\HSDSPerFrameCBuffer.h"
#include "Graphics\terrain\terrain.h"
#include "Math\Plane.h"
#include "Graphics\D3D12Renderer.h"

namespace DE
{

SceneGraph* SceneGraph::m_pInstance;

SceneGraph::SceneGraph()
	: m_tree(0)
	, DEBUG_DRAWING_TREE(0)
{
	D3D12Renderer* renderer = Renderer::GetInstance();
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


	m_ShadowPass = new RenderPass;
	m_ShadowPass->SetPixelShader(nullptr);
	m_ShadowPass->SetBlendState(State::NULL_STATE);
	m_ShadowPass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);
}

void SceneGraph::FrustumCulling(Frustum frustum)
{
	const unsigned int size = m_tree.Size();
	for (int i = 0; i < size; ++i)
	{
		AABB meshBound = m_tree[i]->m_pMeshData->GetBoundingBox();
		AABB cache = meshBound;
		Matrix4 cameraSpace = Renderer::GetInstance()->GetCamera()->GetViewMatrix() * *m_tree[i]->GetOwner()->GetTransform();
		meshBound.Transform(cameraSpace);
		if (frustum.Cull(meshBound))
		{
			m_tree[i]->m_bVisible = true;
		}
		else
		{
			m_tree[i]->m_bVisible = false;
		}
	}
}

void SceneGraph::Render(Renderer* renderer)
{
	renderer->m_pCommandList->SetGraphicsRootSignature(m_pRootSignature);

	// TODO: use better gpu recources management
	m_VSCBuffer.BindToRenderer();
	m_MatrixPalette.BindToRenderer();
	m_PSCBuffer.BindToRenderer();

	int count = 0; //
	const unsigned int size = m_tree.Size();
	for (int i = 0; i < size; ++i)
	{
		MeshComponent* itr = m_tree[i];
		if (itr->m_bVisible) count++; //

		VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_VSCBuffer.m_Memory._data;
		ptr->WorldTransform = renderer->GetCamera()->GetViewMatrix() * *itr->m_pTransform;
		ptr->WVPTransform = renderer->GetCamera()->GetPVMatrix() * *itr->m_pTransform;
		m_VSCBuffer.Update();
		m_VSCBuffer.BindToRendererWithOffset(0, i); // 0 is VS per object

		AnimationController* anim = itr->GetOwner()->GetComponent<AnimationController>();
		if (anim != nullptr)
		{
			Skeleton* skel = itr->GetOwner()->GetComponent<Skeleton>();
			VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER* palette = (VSMatrixPaletteCBuffer::VS_MATRIX_PALETTE_CBUFFER*) m_MatrixPalette.m_Memory._data;
			const int jointCount = anim->GetAnimationSetCount();
			anim->GetAnimationSets().ForEachItem([skel, palette](AnimationSet* item)
			{
				if (item->isActive())
				{
					for (int index = 0; index < skel->GetJointsCount(); ++index)
					{
						palette->mSkinning[index] = *skel->m_vGlobalPose[index] * skel->m_vJoints[index].m_mBindPoseInv;
					}
				}
			}); 
			m_MatrixPalette.Update();
			m_MatrixPalette.BindToRendererWithOffset(1, i); // 1 is VS matrix palatte
		}

		PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER* ptr2 = (PSPerMaterialCBuffer::PS_PER_MATERIAL_CBUFFER*) m_PSCBuffer.m_Memory._data;
		ptr2->material.vSpecular = itr->m_pMeshData->m_Material.GetSpecular();
		ptr2->material.fShininess = itr->m_pMeshData->m_Material.GetShininess();
		m_PSCBuffer.Update();
		m_PSCBuffer.BindToRendererWithOffset(2, i); // 2 is PS material

		itr->Draw(renderer);
	}
}

void SceneGraph::ShadowMapGeneration()
{
	for (int i = 0; i < LightManager::GetInstance()->GetNumLights(); i++)
	{
		LightComponent* currLight = LightManager::GetInstance()->GetLightAt(i);
		if (currLight->IsCastShadow())
		{
			// TODO: implement D3D12 shadow mapping
		}
	}
}

void SceneGraph::AddComponent(MeshComponent * meshComponent)
{
	m_tree.Add(meshComponent);
}

void SceneGraph::ADD_DEBUG_DRAWING(MeshComponent * meshComponent)
{
	DEBUG_DRAWING_TREE.Add(meshComponent);
}

};
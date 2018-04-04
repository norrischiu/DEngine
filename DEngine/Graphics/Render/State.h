#ifndef STATE_H_
#define STATE_H_

// Engine include
#include "GlobalInclude.h"

// D3D inclued
#include <assert.h>
#include <d3d12.h>

/*
*	NAMESPACE: State
*	Information enclosed in this namespace defined the default
*	D3D11 states used in this engine. For example, depth stencil
*	state, rasterizer state and blend state. This can be extended
*	to allow more default state
*/
namespace State
{

	/*
	*	ENUM
	*	The name of the state
	*/
	enum
	{
		NULL_STATE,
		DEFAULT_DEPTH_STENCIL_DSS,
		DISABLE_DEPTH_STENCIL_DSS,
		DISABLE_DEPTH_DISABLE_STENCIL_DSS,
		GBUFFER_STENCIL_CHECK_DSS,
		CULL_FRONT_RS,
		CULL_BACK_RS,
		CULL_NONE_RS,
		WIREFRAME_RS,
		DEFAULT_BS,
		ADDITIVE_BS,
		ALPHA_BS,
		LINEAR_MIPMAP_MAX_LOD_SS
	};

	struct D3D12_STATE_DESC
	{
		union 
		{
			D3D12_DEPTH_STENCIL_DESC DS;
			D3D12_RASTERIZER_DESC RS;
			D3D12_BLEND_DESC BS;
			D3D12_STATIC_SAMPLER_DESC SS;
		};
	};

	extern D3D12_STATE_DESC			m_States[20];		// array of all default states

	/********************************************************************************
	*	--- Static Function:
	*	ConstructDefaultStates(ID3D11Device*)
	*	This function will create all default D3D11 states used in this engine
	*
	*	--- Parameters:
	*	@ pDevice: the pointer to the D3D11 device
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	static void ConstructDefaultStates()
	{
		HRESULT hr;
		D3D12_STATE_DESC stateDesc;

		D3D12_DEPTH_STENCIL_DESC depthStencilDesc;
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthEnable = true; // true
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = 0;
		depthStencilDesc.StencilWriteMask = 0;
		D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
		{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
		depthStencilDesc.FrontFace = defaultStencilOp;
		depthStencilDesc.BackFace = defaultStencilOp;
		stateDesc.DS = depthStencilDesc;
		m_States[DEFAULT_DEPTH_STENCIL_DSS] = stateDesc;

		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = 0x00;
		depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_ZERO;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_ZERO;
		depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
		depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		depthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_ZERO;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_ZERO;
		depthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
		depthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		stateDesc.DS = depthStencilDesc;
		m_States[DISABLE_DEPTH_STENCIL_DSS] = stateDesc;

		depthStencilDesc.StencilEnable = false;
		stateDesc.DS = depthStencilDesc;
		m_States[DISABLE_DEPTH_DISABLE_STENCIL_DSS] = stateDesc;

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
		depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
		depthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		stateDesc.DS = depthStencilDesc;
		m_States[GBUFFER_STENCIL_CHECK_DSS] = stateDesc;

		// Rasterizer state
		D3D12_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		stateDesc.RS = rasterizerDesc;
		m_States[CULL_BACK_RS] = stateDesc;

		rasterizerDesc.CullMode = D3D12_CULL_MODE_FRONT;
		stateDesc.RS = rasterizerDesc;
		m_States[CULL_FRONT_RS] = stateDesc;

		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		stateDesc.RS = rasterizerDesc;
		m_States[CULL_NONE_RS] = stateDesc;

		rasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
		stateDesc.RS = rasterizerDesc;
		m_States[WIREFRAME_RS] = stateDesc;

		// Blend state
		D3D12_BLEND_DESC blendStateDesc;
		blendStateDesc.AlphaToCoverageEnable = FALSE;
		blendStateDesc.IndependentBlendEnable = FALSE;
		blendStateDesc.RenderTarget[0].BlendEnable = FALSE;
		blendStateDesc.RenderTarget[0].LogicOpEnable = FALSE;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		stateDesc.BS = blendStateDesc;
		m_States[DEFAULT_BS] = stateDesc;

		ZeroMemory(&blendStateDesc, sizeof(D3D12_BLEND_DESC));
		blendStateDesc.AlphaToCoverageEnable = FALSE;
		blendStateDesc.IndependentBlendEnable = FALSE;
		blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
		blendStateDesc.RenderTarget[0].LogicOpEnable = FALSE;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		blendStateDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		stateDesc.BS = blendStateDesc;
		m_States[ADDITIVE_BS] = stateDesc;

		ZeroMemory(&blendStateDesc, sizeof(D3D12_BLEND_DESC));
		blendStateDesc.AlphaToCoverageEnable = FALSE;
		blendStateDesc.IndependentBlendEnable = FALSE;
		blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		blendStateDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		stateDesc.BS = blendStateDesc;
		m_States[ALPHA_BS] = stateDesc;

		// Set sampler state
		D3D12_STATIC_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		samplerDesc.ShaderRegister = 0;
		samplerDesc.RegisterSpace = 0;
		stateDesc.SS = samplerDesc;
		m_States[LINEAR_MIPMAP_MAX_LOD_SS] = stateDesc;
	}

	/********************************************************************************
	*	--- Static Function:
	*	GetState(int)
	*	This function will return the state located at the given ID
	*
	*	--- Parameters:
	*	@ stateID: the state ID as in the enum
	*
	*	--- Return:
	*	@ void*: pointer to the specific state
	********************************************************************************/
	static D3D12_STATE_DESC GetState(int stateID)
	{
		return m_States[stateID];
	}

};

#endif // !STATE_H_

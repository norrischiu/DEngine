#ifndef STATE_H_
#define STATE_H_

#include <assert.h>
#include <d3d11.h>

namespace State
{

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
		ADDITIVE_BS,
		ALPHA_BS,
		LINEAR_MIPMAP_MAX_LOD_SS
	};

	extern void*					m_States[20];

	static void ConstructDefaultStates(ID3D11Device* pDevice)
	{
		HRESULT hr;

		// Depth stencil state
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		depthStencilDesc.DepthEnable = true; // true
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = 0;
		depthStencilDesc.StencilWriteMask = 0;

		hr = pDevice->CreateDepthStencilState(&depthStencilDesc, (ID3D11DepthStencilState**)&m_States[DEFAULT_DEPTH_STENCIL_DSS]);
		assert(hr == S_OK);
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = 0x00;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = pDevice->CreateDepthStencilState(&depthStencilDesc, (ID3D11DepthStencilState**)&m_States[DISABLE_DEPTH_STENCIL_DSS]);
		assert(hr == S_OK);
		depthStencilDesc.StencilEnable = false;
		hr = pDevice->CreateDepthStencilState(&depthStencilDesc, (ID3D11DepthStencilState**)&m_States[DISABLE_DEPTH_DISABLE_STENCIL_DSS]);
		assert(hr == S_OK);

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		hr = pDevice->CreateDepthStencilState(&depthStencilDesc, (ID3D11DepthStencilState**)&m_States[GBUFFER_STENCIL_CHECK_DSS]);
		assert(hr == S_OK);

		// Rasterizer state
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		hr = pDevice->CreateRasterizerState(&rasterizerDesc, (ID3D11RasterizerState**)&m_States[CULL_BACK_RS]);
		assert(hr == S_OK);
		rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		hr = pDevice->CreateRasterizerState(&rasterizerDesc, (ID3D11RasterizerState**)&m_States[CULL_FRONT_RS]);
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		assert(hr == S_OK);
		hr = pDevice->CreateRasterizerState(&rasterizerDesc, (ID3D11RasterizerState**)&m_States[CULL_NONE_RS]);
		assert(hr == S_OK);
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		hr = pDevice->CreateRasterizerState(&rasterizerDesc, (ID3D11RasterizerState**)&m_States[WIREFRAME_RS]);
		assert(hr == S_OK);

		// Blend state
		D3D11_BLEND_DESC blendStateDesc;
		blendStateDesc.AlphaToCoverageEnable = FALSE;
		blendStateDesc.IndependentBlendEnable = FALSE;
		blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = pDevice->CreateBlendState(&blendStateDesc, (ID3D11BlendState**)&m_States[ADDITIVE_BS]);
		assert(hr == S_OK);

		ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
		blendStateDesc.AlphaToCoverageEnable = FALSE;
		blendStateDesc.IndependentBlendEnable = FALSE;
		blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = pDevice->CreateBlendState(&blendStateDesc, (ID3D11BlendState**)&m_States[ALPHA_BS]);
		assert(hr == S_OK);

		// Set sampler state
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = pDevice->CreateSamplerState(&samplerDesc, (ID3D11SamplerState**)&m_States[LINEAR_MIPMAP_MAX_LOD_SS]);
		assert(hr == S_OK);
	}

	static void* GetState(int stateID)
	{
		return m_States[stateID];
	}

};

#endif // !STATE_H_

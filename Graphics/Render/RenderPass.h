#ifndef RENDER_PASS_H_
#define RENDER_PASS_H_

#include <d3d11.h>
#include <assert.h>
#include <vector>
#include "ShaderManager.h"
#include "State.h"
#include "Texture.h"

class RenderPass
{

public:

	RenderPass() 
	{
		m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_iRTVNum = 0;
	}

	void SetVertexShader(const char* filename)
	{
		assert(filename != nullptr);

		m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader(filename, D3D11_SHVER_VERTEX_SHADER);
		m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout(filename);
	}

	void SetPixelShader(const char* filename)
	{
		if (filename == nullptr)
		{
			m_pPS = nullptr;
		}
		else
		{
			m_pPS = (ID3D11PixelShader*)ShaderManager::GetInstance()->GetShader(filename, D3D11_SHVER_PIXEL_SHADER);
		}
	}

	void SetRasterizerState(int stateID)
	{
		m_pRS = (ID3D11RasterizerState*) State::GetState(stateID);
	}

	void SetDepthStencilState(int stateID)
	{
		m_pDSS = (ID3D11DepthStencilState*) State::GetState(stateID);
	}

	void SetBlendState(int stateID)
	{
		m_pBS = (ID3D11BlendState*) State::GetState(stateID);
	}
	
	void SetTopology(int topology)
	{
		m_iTopology = topology;
	}

	void SetRenderTargets(ID3D11RenderTargetView** RTVs, int num)
	{
		m_pRTVs = RTVs;
		m_iRTVNum = num;
	}

	void SetDepthStencilView(ID3D11DepthStencilView* DSV)
	{
		m_pDSV = DSV;
	}

	void AddTexture(Texture* tex)
	{
		m_vTextureSRVs.push_back(tex->GetSRV());
		m_vSamplerState.push_back(tex->GetSamplerState());
	}

	void PopTexture()
	{
		m_vTextureSRVs.pop_back();
		m_vSamplerState.pop_back();
	}

	void BindToRenderer();

private:

	// Pointer to complied to vertex shader
	ID3D11VertexShader*						m_pVS;

	// Pointer to complied to pixel shader
	ID3D11PixelShader*						m_pPS;

	// Primitive Topology
	unsigned int							m_iTopology;

	// Pointer to input layout supply to IA
	ID3D11InputLayout*						m_pInputLayout;

	// Pointer to depth stencil state
	ID3D11DepthStencilState*				m_pDSS;

	// Pointer to blend state
	ID3D11BlendState*						m_pBS;

	// Pointer to rasterizer state
	ID3D11RasterizerState*					m_pRS;

	ID3D11DepthStencilView*					m_pDSV;

	ID3D11RenderTargetView**				m_pRTVs;

	int										m_iRTVNum;

	// Pointer to texture array
	std::vector<ID3D11ShaderResourceView*>	m_vTextureSRVs;

	std::vector<ID3D11SamplerState*>		m_vSamplerState;
};

#endif // RENDER_PASS_H_


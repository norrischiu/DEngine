#ifndef RENDER_PASS_H_
#define RENDER_PASS_H_

// D3D11 include
#include <d3d11.h>

// Engine include
#include "Utilities\MyArray.h"
#include "Graphics\ShaderManager.h"
#include "State.h"
#include "Texture.h"

// C++ include
#include <assert.h>

namespace DE
{

/*
*	CLASS: RenderPass
*	RenderPass contains all the data required to render a mesh,
*	including all pointers to shader, state, resources view and
*	topology at the GPU side. This can be used solely or added
*	to a existing render technique. This will be used everytime
*	before D3D11 render call
*/
class RenderPass
{

public:

	/********************************************************************************
	*	--- Constructor:
	*	RenderPass()
	*	This constructor will construct a empty render pass with default topology as
	*	triangle list
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	RenderPass()
		: m_vTextureSRVs(0)
		, m_vSamplerState(0)
	{
		m_iTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_iRTVNum = 0;
	}

	/********************************************************************************
	*	--- Destructor:
	*	~RenderTechnique()
	*	This destructor will free the memory used by the shader resources array and
	*	sampler state array
	********************************************************************************/
	~RenderPass()
	{
	}

	/********************************************************************************
	*	--- Function:
	*	SetVertexShader(ID3D11VertexShader*)
	*	This function will stores pointer to the D3D11 vertex shader in this 
	*	render pass
	*
	*	--- Parameters:
	*	@ pVS: the pointer to D3D11 vertex shader
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetVertexShader(ID3D11VertexShader* pVS)
	{
		m_pVS = pVS;
	}

	/********************************************************************************
	*	--- Function:
	*	SetVertexShader(const char*)
	*	This function will accept a filename and find the according vertex shader
	*	pointer from the shader manager, this also sets the input layout with
	*	reference to this vertex shader
	*
	*	--- Parameters:
	*	@ filename: the vertex shader filename
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetVertexShader(const char* filename)
	{
		assert(filename != nullptr);

			m_pVS = (ID3D11VertexShader*)ShaderManager::GetInstance()->GetShader(filename, D3D11_SHVER_VERTEX_SHADER);
			m_pInputLayout = (ID3D11InputLayout*)ShaderManager::GetInstance()->GetInputLayout(filename);
	}

	/********************************************************************************
	*	--- Function:
	*	GetVertexShader()
	*	This function will return pointer to the D3D11 vertex shader in this
	*	render pass
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ ID3D11VertexShader*: the pointer to the vertex shader at GPU side
	********************************************************************************/
	ID3D11VertexShader* GetVertexShader()
	{
		return m_pVS;
	}

	/********************************************************************************
	*	--- Function:
	*	SetPixelShader(const char*)
	*	This function will accept a filename and find the according pixel shader
	*	pointer from the shader manager. It sets the pixel shader as nullptr if the
	*	input is nullptr also, this is a valid operation as a pixel shader is not
	*	compulsory to render a mesh
	*
	*	--- Parameters:
	*	@ filename: the pixel shader filename
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
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

	/********************************************************************************
	*	--- Function:
	*	SetHullShader(const char*)
	*	This function will accept a filename and find the according hull shader
	*	pointer from the shader manager. It sets the hull shader as nullptr if the
	*	input is nullptr also, this is a valid operation as a hull shader is not
	*	compulsory to render a mesh
	*
	*	--- Parameters:
	*	@ filename: the hull shader filename
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetHullShader(const char* filename)
	{
		if (filename == nullptr)
		{
			m_pHS = nullptr;
		}
		else
		{
			m_pHS = (ID3D11HullShader*)ShaderManager::GetInstance()->GetShader(filename, D3D11_SHVER_HULL_SHADER);
		}
	}

	/********************************************************************************
	*	--- Function:
	*	SetDomainShader(const char*)
	*	This function will accept a filename and find the according domain shader
	*	pointer from the shader manager. It sets the domain shader as nullptr if the
	*	input is nullptr also, this is a valid operation as a domain shader is not
	*	compulsory to render a mesh
	*
	*	--- Parameters:
	*	@ filename: the domain shader filename
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetDomainShader(const char* filename)
	{
		if (filename == nullptr)
		{
			m_pDS = nullptr;
		}
		else
		{
			m_pDS = (ID3D11DomainShader*)ShaderManager::GetInstance()->GetShader(filename, D3D11_SHVER_DOMAIN_SHADER);
		}
	}

	/********************************************************************************
	*	--- Function:
	*	SetGeometryShader(const char*)
	*	This function will accept a filename and find the according geometry shader
	*	pointer from the shader manager. It sets the geometry shader as nullptr if the
	*	input is nullptr also, this is a valid operation as a geometry shader is not
	*	compulsory to render a mesh
	*
	*	--- Parameters:
	*	@ filename: the geometry shader filename
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetGeometryShader(const char* filename)
	{
		if (filename == nullptr)
		{
			m_pGS = nullptr;
		}
		else
		{
			m_pGS = (ID3D11GeometryShader*)ShaderManager::GetInstance()->GetShader(filename, D3D11_SHVER_GEOMETRY_SHADER);
		}
	}

	/********************************************************************************
	*	--- Function:
	*	SetStreamOutTargets(ID3D11Buffer*)
	*	This function will set a D3D11 buffer as the stream out target of this 
	*	render pass
	*
	*	--- Parameters:
	*	@ SOBuffer: the pointer to D3D11 buffer to be used as stream out target
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetStreamOutTargets(ID3D11Buffer* SOBuffer)
	{
		m_pSOTarget = SOBuffer;
	}

	/********************************************************************************
	*	--- Function:
	*	SetRasterizerState(int)
	*	This function will set the rasterizer state of this render pass
	*
	*	--- Parameters:
	*	@ stateID: the index of the state as defined inside namespace State
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetRasterizerState(int stateID)
	{
		m_pRS = (ID3D11RasterizerState*)State::GetState(stateID);
	}

	/********************************************************************************
	*	--- Function:
	*	SetDepthStencilState(int)
	*	This function will set the depth stencil state of this render pass
	*
	*	--- Parameters:
	*	@ stateID: the index of the state as defined inside namespace State
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetDepthStencilState(int stateID)
	{
		m_pDSS = (ID3D11DepthStencilState*)State::GetState(stateID);
	}


	/********************************************************************************
	*	--- Function:
	*	SetBlendState(int)
	*	This function will set the blend state of this render pass
	*
	*	--- Parameters:
	*	@ stateID: the index of the state as defined inside namespace State
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetBlendState(int stateID)
	{
		m_pBS = (ID3D11BlendState*)State::GetState(stateID);
	}

	/********************************************************************************
	*	--- Function:
	*	SetTopology(int)
	*	This function will set the topology for Input-Assembly stage of this render pass
	*
	*	--- Parameters:
	*	@ topology: the toplogy as defined in D3D11 topology enum
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetTopology(int topology)
	{
		m_iTopology = topology;
	}

	/********************************************************************************
	*	--- Function:
	*	SetRenderTargets(ID3D11RenderTargetView**, int)
	*	This function will set the render targets for this render pass
	*
	*	--- Parameters:
	*	@ RTVs: an array of render target views pointer or the address of a single
	*	render targe view
	*	@ num: number of items in the given array or intended number of render target
	*	view to be used from the array
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetRenderTargets(ID3D11RenderTargetView** RTVs, int num)
	{
		m_pRTVs = RTVs;
		m_iRTVNum = num;
	}

	/********************************************************************************
	*	--- Function:
	*	SetDepthStencilView(ID3D11DepthStencilView* DSV)
	*	This function will set the depth stencil view in this render pass
	*
	*	--- Parameters:
	*	@ DSV: a pointer to D3D11 depth stencil view
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetDepthStencilView(ID3D11DepthStencilView* DSV)
	{
		m_pDSV = DSV;
	}

	/********************************************************************************
	*	--- Function:
	*	AddTexture(Handle)
	*	This function will add a texture to the render pass, and the sampler state 
	*	linked to the texture
	*
	*	--- Parameters:
	*	@ hTex: a handle referring to a Texture class
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void AddTexture(Handle hTex)
	{
		m_vTextureSRVs.Add(((Texture*)hTex.Raw())->GetSRV());
		m_vSamplerState.Add(((Texture*)hTex.Raw())->GetSamplerState());
	}

	/********************************************************************************
	*	--- Function:
	*	AddTexture(Texture*)
	*	This function will add a texture to the render pass, and the sampler state 
	*	linked to the texture
	*
	*	--- Parameters:
	*	@ tex: a pointer to a Texture class
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void AddTexture(Texture* tex)
	{
		m_vTextureSRVs.Add(tex->GetSRV());
		m_vSamplerState.Add(tex->GetSamplerState());
	}

	/********************************************************************************
	*	--- Function:
	*	PopTexture()
	*	This function will pop a texture from the array (remove the latest added 
	*	texture), and pop a sampler state as well
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void PopTexture()
	{
		m_vTextureSRVs.Pop();
		m_vSamplerState.Pop();
	}

	/********************************************************************************
	*	--- Function:
	*	GetTextureCount()
	*	This function will return the number of texture added to this render pass
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ int: the number of textures in the texture array
	********************************************************************************/
	int GetTextureCount()
	{
		return m_vTextureSRVs.Size();
	}

	/********************************************************************************
	*	--- Function:
	*	BindToRenderer()
	*	This function will bind the pass to the GPU, i.e. setting all configuration
	*	in this pass to the renderer through D3D11 calls
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void BindToRenderer();

private:

	
	ID3D11VertexShader*						m_pVS;		// Pointer to complied vertex shader
	ID3D11HullShader*						m_pHS;		// Pointer to complied hull shader
	ID3D11DomainShader*						m_pDS;		// Pointer to compiled domain shader
	ID3D11GeometryShader*					m_pGS;		// Pointer to complied geometry shader	
	ID3D11PixelShader*						m_pPS;		// Pointer to complied pixel shader
	unsigned int							m_iTopology;	// Primitive Topology
	ID3D11InputLayout*						m_pInputLayout;		// Pointer to input layout supply to IA
	ID3D11DepthStencilState*				m_pDSS;		// Pointer to depth stencil state
	ID3D11BlendState*						m_pBS;		// Pointer to blend state
	ID3D11RasterizerState*					m_pRS;		// Pointer to rasterizer state
	ID3D11DepthStencilView*					m_pDSV;		// Pointer to depth stencil state
	ID3D11RenderTargetView**				m_pRTVs;	// Array of render targets or address of single render target
	ID3D11Buffer*							m_pSOTarget;	// Pointer to a buffer as stream out target
	int										m_iRTVNum;		// Number of render targets
	MyArray<ID3D11ShaderResourceView*>		m_vTextureSRVs;		// texture array
	MyArray<ID3D11SamplerState*>			m_vSamplerState;	// sampler state array
};

};
#endif // RENDER_PASS_H_


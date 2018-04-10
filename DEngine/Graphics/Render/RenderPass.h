#ifndef RENDER_PASS_H_
#define RENDER_PASS_H_

// D3D12 include
#include <d3d12.h>
#include "Graphics\d3dx12.h"
#include <d3d12shader.h>

// Engine include
#include "Graphics\D3D12Renderer.h"
#include "Utilities\MyArray.h"
#include "Graphics\ShaderManager.h"
#include "State.h"
#include "Texture.h"
#include "GlobalInclude.h"

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
*	before D3D render call
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
	{
		m_iTopology = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		m_iRTVNum = 0;
	};

	/********************************************************************************
	*	--- Destructor:
	*	~RenderTechnique()
	*	This destructor will free the memory used by the shader resources array and
	*	sampler state array
	********************************************************************************/
	~RenderPass()
	{
	}

	void ConstructPSO();

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
		m_pVS = (ID3DBlob*)ShaderManager::GetInstance()->GetShader(filename, D3D12_SHVER_VERTEX_SHADER);
		m_InputLayout = ShaderManager::GetInstance()->GetInputLayout(filename);
		m_InputLayoutCount = ShaderManager::GetInstance()->GetInputLayoutCount(filename);
	}

	/********************************************************************************
	*	--- Function:
	*	GetVertexShader()
	*	This function will return pointer to the D3D vertex shader in this
	*	render pass
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void*: the pointer to the vertex shader at GPU side
	********************************************************************************/
	void* GetVertexShader() 
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
			m_pPS = ShaderManager::GetInstance()->GetShader(filename, D3D12_SHVER_PIXEL_SHADER);
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
			m_pHS = ShaderManager::GetInstance()->GetShader(filename, D3D12_SHVER_HULL_SHADER);
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
			m_pDS = ShaderManager::GetInstance()->GetShader(filename, D3D12_SHVER_DOMAIN_SHADER);
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
			m_pGS = ShaderManager::GetInstance()->GetShader(filename, D3D12_SHVER_GEOMETRY_SHADER);
		}
	}

	/********************************************************************************
	*	--- Function:
	*	SetStreamOutTargets(D3D12_STREAM_OUTPUT_BUFFER_VIEW*)
	*	This function will set a D3D buffer as the stream out target of this 
	*	render pass
	*
	*	--- Parameters:
	*	@ SOBuffer: the pointer to D3D buffer to be used as stream out target
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetStreamOutTargets(D3D12_STREAM_OUTPUT_BUFFER_VIEW* SOBuffer)
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
		m_iRS = stateID;
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
		m_iDS = stateID;
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
		m_iBS = stateID;
	}

	/********************************************************************************
	*	--- Function:
	*	SetTopology(int)
	*	This function will set the topology for Input-Assembly stage of this render pass
	*
	*	--- Parameters:
	*	@ topology: the toplogy as defined in D3D topology enum
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
	*	SetRenderTargets(Texture**, int)
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
	void SetRenderTargets(Texture** RTVs, int num)
	{
		m_pRTVs = RTVs;
		m_iRTVNum = num;
	}

	/********************************************************************************
	*	--- Function:
	*	SetDepthStencilView(Texture* DSV)
	*	This function will set the depth stencil view in this render pass
	*
	*	--- Parameters:
	*	@ DSV: a pointer to Texture
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void SetDepthStencilView(Texture* DSV)
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
		m_iSS = ((Texture*)hTex.Raw())->GetSamplerDescIndex();
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
		m_iSS = State::LINEAR_MIPMAP_MAX_LOD_SS;
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
	*	in this pass to the renderer through D3D calls
	*
	*	--- Parameters:
	*	@ Renderer*: pointer to renderer
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void BindToRenderer(Renderer* renderer);

private:

	ID3D12RootSignature*					m_pRootSignature; // root signature for D3D12 render
	ID3D12PipelineState*					m_pPSO; // Pointer to pipeline state object for D3D12 render
	void*									m_pVS;		// Pointer to vertex shader bytecode
	void*									m_pHS;		// Pointer to hull shader bytecode
	void*									m_pDS;		// Pointer to domain shader bytecode
	void*									m_pGS;		// Pointer to geometry shader bytecode	
	void*									m_pPS;		// Pointer to pixel shader bytecode
	D3D12_INPUT_ELEMENT_DESC*				m_InputLayout;
	int										m_iDS;
	int										m_iRS;
	int										m_iBS;
	int										m_iSS;
	Texture*								m_pDSV;
	D3D12_STREAM_OUTPUT_BUFFER_VIEW*		m_pSOTarget;	// Pointer to a buffer as stream out target
	MyArray<D3D12_CPU_DESCRIPTOR_HANDLE>	m_vTextureSRVs;		// texture array
	Texture**								m_pRTVs;
	int										m_InputLayoutCount;

	unsigned int							m_iTopology;	// Primitive Topology
	int										m_iRTVNum;		// Number of render targets	

};

};
#endif // RENDER_PASS_H_


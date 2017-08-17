#ifndef TEXTURE_H_
#define TEXTURE_H_

// D3D include
#include <d3d11.h>
#include <d3d12.h>

// Engine include
#include "Memory\Handle.h"
#include "GlobalInclude.h"

// C++ include
#include <assert.h>

namespace DE
{

/*
*	CLASS: Texture
*	This class serves as an interface and collection for GPU texture
*	resources, it can contains different kind of D3D11 view resources.
*	Currently it also contains pointer to the sampler state
*/
class Texture
{

public:

	CUSTOM_MEMORY_DEFINE();

	/*
	*	ENUM
	*	Serves as bitfield for type of GPU texture resources
	*/
	enum
	{
		DEPTH_STENCIL = 0x01,
		DEPTH_STENCIL_READ_ONLY = 0x02,
		SHADER_RESOURCES = 0x04,
		RENDER_TARGET = 0x08,
		UNORDER_ACCESS = 0x10,
	};

	/********************************************************************************
	*	--- Constructor:
	*	Texture(int, int, const char*, int);
	*	This constructor will construct a texture interface with any given source
	*	file, and mips level configuration
	*
	*	--- Parameters:
	*	@ type: the type of this texture as defined in the enum
	*	@ sampleCount: the sample count used for MSAA configuration
	*	@ filename: the source file of the texture in DDS format
	*	@ mipLevel: the mips level of the texture
	********************************************************************************/
	Texture(int type, int sampleCount = 1, const char* filename = nullptr, int mipLevel = 1);

	/********************************************************************************
	*	--- Constructor:
	*	Texture(int, ID3D11Texture2D*);
	*	This constructor will construct an texture interface with a given pointer to
	*	a GPU 2D texture
	*
	*	--- Parameters:
	*	@ type: the type of this texture as defined in the enum
	*	@ texSrc: the texture pointer already defined in GPU
	********************************************************************************/
#ifdef D3D12
	Texture(int type, ID3D12Resource* texSrc);
#elif defined D3D11
	Texture(int type, ID3D11Texture2D* texSrc);
#endif

	/********************************************************************************
	*	--- Function:
	*	GetSRV()
	*	This function will return the created shader resource view. This will throw
	*	an assertion if it is null
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ ID3D11ShaderResourceView*: pointer to a D3D11 shader resource view
	********************************************************************************/
#ifdef D3D12
	D3D12_CPU_DESCRIPTOR_HANDLE	GetSRV()
#elif defined D3D11
	ID3D11ShaderResourceView*& GetSRV()
#endif
	{
		return m_pSRV;
	}

	/********************************************************************************
	*	--- Function:
	*	GetRTV()
	*	This function will return the created render target view. This will throw
	*	an assertion if it is null
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ ID3D11ShaderResourceView*&: reference of pointer to a D3D11 render
	*	target view
	********************************************************************************/
#ifdef D3D12
	D3D12_CPU_DESCRIPTOR_HANDLE	GetRTV()
#elif defined D3D11
	ID3D11RenderTargetView*& GetRTV()
#endif
	{
		return m_pRTV;
	}

	/********************************************************************************
	*	--- Function:
	*	GetDSV()
	*	This function will return the created depth stencil view. This will throw
	*	an assertion if it is null
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ ID3D11DepthStencilView*: pointer to a D3D11 depth stencil view
	********************************************************************************/
#ifdef D3D12
	ID3D12DescriptorHeap*	GetDSV()
#elif defined D3D11
	ID3D11DepthStencilView*	GetDSV()
#endif
	{
		assert(m_pDSV != nullptr);
		return m_pDSV;
	}

	/********************************************************************************
	*	--- Function:
	*	GetTexture2D()
	*	This function will return the stored 2D texture
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ ID3D11Texture2D*: pointer to a D3D11 texture 2D
	********************************************************************************/
#ifdef D3D12
	ID3D12Resource* GetTexture2D()
#elif defined D3D11
	ID3D11Texture2D* GetTexture2D()
#endif
	{
		return m_pTexture;
	}

	/********************************************************************************
	*	--- Function:
	*	GetSamplerState()
	*	This function will return the created sampler state
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ ID3D11SamplerState*: pointer to a D3D11 sampler state
	********************************************************************************/
#ifdef D3D12
	int GetSamplerDescIndex()
	{
		return m_iSampler;
	}
#elif defined D3D11
	ID3D11SamplerState* GetSamplerState()
	{
		return m_pSampler;
	}
#endif

	DXGI_FORMAT GetFormat()
	{
		return m_Format;
	}

private:

	int											m_type;		// type of GPU texture resources stored
	DXGI_FORMAT									m_Format;

#ifdef D3D12
	union
	{
		D3D12_CPU_DESCRIPTOR_HANDLE				m_pRTV;		// pointer to D3D11 render targer view, union with m_pDSV
		ID3D12DescriptorHeap*					m_pDSV;		// pointer to D3D11 depth stencil view, union with m_pRTV
	};
	union
	{
		D3D12_CPU_DESCRIPTOR_HANDLE				m_pUAV;
		D3D12_CPU_DESCRIPTOR_HANDLE				m_pSRV;
	};
	int											m_iSampler;
	ID3D12Resource*								m_pTexture;		// pointer to D3D12 texture 2D
#elif defined D3D11
	union
	{
		ID3D11RenderTargetView*					m_pRTV;		// pointer to D3D11 render targer view, union with m_pDSV
		ID3D11DepthStencilView*					m_pDSV;		// pointer to D3D11 depth stencil view, union with m_pRTV
	};
	union
	{
		ID3D11UnorderedAccessView*				m_pUAV;		// pointer to D3D11 unordered access view, union with m_pSRV
		ID3D11ShaderResourceView*				m_pSRV;		// pointer to D3D11 shader resources view, union with m_pUAV
	};
	ID3D11SamplerState*							m_pSampler;		// pointer to D3D11 sampler state
	ID3D11Texture2D*							m_pTexture;		// pointer to D3D11 texture 2D
#endif
};

};
#endif // !TEXTURE_H_

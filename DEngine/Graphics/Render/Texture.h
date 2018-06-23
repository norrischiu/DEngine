#ifndef TEXTURE_H_
#define TEXTURE_H_

// D3D include
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
*	resources, it can contains different kind of D3D view resources.
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
	*	Texture(int, ID3D12Resource*);
	*	This constructor will construct an texture interface with a given pointer to
	*	a GPU 2D texture
	*
	*	--- Parameters:
	*	@ type: the type of this texture as defined in the enum
	*	@ texSrc: the texture pointer already defined in GPU
	********************************************************************************/
	Texture(int type, ID3D12Resource* texSrc);

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
	*	@ D3D12_CPU_DESCRIPTOR_HANDLE: D3D shader resource view descriptor
	********************************************************************************/
	D3D12_CPU_DESCRIPTOR_HANDLE	GetSRV()
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
	*	@ D3D12_CPU_DESCRIPTOR_HANDLE: D3D render target view descriptor
	********************************************************************************/
	D3D12_CPU_DESCRIPTOR_HANDLE	GetRTV()
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
	*	@ D3D12_CPU_DESCRIPTOR_HANDLE: D3D depth stencil view descriptor
	********************************************************************************/
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSV() // TODO: fix this
	{
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
	*	@ ID3D12Resource*: pointer to a D3D texture 2D
	********************************************************************************/
	ID3D12Resource* GetTexture2D()
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
	*	@ int: index of the sampler state in default state array
	********************************************************************************/
	int GetSamplerDescIndex()
	{
		return m_iSampler;
	}

	DXGI_FORMAT GetFormat()
	{
		return m_Format;
	}

private:

	int											m_type;		// type of GPU texture resources stored
	DXGI_FORMAT									m_Format;

	union
	{
		D3D12_CPU_DESCRIPTOR_HANDLE				m_pRTV;		// pointer to D3D render targer view, union with m_pDSV
		D3D12_CPU_DESCRIPTOR_HANDLE				m_pDSV;		// pointer to D3D depth stencil view, union with m_pRTV
	};
	union
	{
		D3D12_CPU_DESCRIPTOR_HANDLE				m_pUAV;
		D3D12_CPU_DESCRIPTOR_HANDLE				m_pSRV;
	};
	int											m_iSampler;
	ID3D12Resource*								m_pTexture;		// pointer to D3D12 texture 2D
};

};
#endif // !TEXTURE_H_

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <d3d11.h>
#include <assert.h>
#include "Memory/Handle.h"

namespace DE
{

class Texture
{

public:

	// Type bitfield
	enum
	{
		DEPTH_STENCIL = 0x01,
		DEPTH_STENCIL_READ_ONLY = 0x02,
		SHADER_RESOURCES = 0x04,
		RENDER_TARGET = 0x08,
		UNORDER_ACCESS = 0x10,
	};

	CUSTOM_MEMORY_DEFINE();

	Texture(int type, int mipsLevel = 1, const char* filename = nullptr);

	Texture(int type, ID3D11Texture2D* texSrc);

	ID3D11ShaderResourceView* GetSRV()
	{
		assert(m_pSRV != nullptr);
		return m_pSRV;
	}

	ID3D11RenderTargetView*& GetRTV()
	{
		assert(m_pRTV != nullptr);
		return m_pRTV;
	}

	ID3D11DepthStencilView*	GetDSV()
	{
		assert(m_pDSV != nullptr);
		return m_pDSV;
	}

	ID3D11Texture2D* GetTexture2D()
	{
		return m_pTexture;
	}

	ID3D11SamplerState* GetSamplerState()
	{
		return m_pSampler;
	}

private:

	int											m_type;

	union
	{
		ID3D11RenderTargetView*					m_pRTV;
		ID3D11DepthStencilView*					m_pDSV;
	};

	union
	{
		ID3D11UnorderedAccessView*				m_pUAV;
		ID3D11ShaderResourceView*				m_pSRV;
	};

	ID3D11SamplerState*							m_pSampler;

	ID3D11Texture2D*							m_pTexture;
};

};
#endif // !TEXTURE_H_

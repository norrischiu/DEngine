#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <stdio.h>
#include <d3d11.h>
#include <d3d12.h>
#include "Utilities\MyHashMap.h"
#include "GlobalInclude.h"

namespace DE
{

/*
*	CLASS: TextureManager
*	TextureManager is the resources manager for texture stored
*	as D3D11 shader resources view
*/
class TextureManager
{
public:

	/********************************************************************************
	*	--- Constructor:
	*	TextureManager()
	*	This constructor will allocate default size of memory for hash map
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	TextureManager()
		: m_mapTexture()
	{
	};

	/********************************************************************************
	*	--- Destructor:
	*	~TextureManager()
	*	This constructor will free memory from hash map and release each D3D11 COM
	*	object
	********************************************************************************/
	~TextureManager()
	{
		m_mapTexture.ForEachItem([](void* item)
		{
			ID3D11ShaderResourceView* pSRView = (ID3D11ShaderResourceView*)item;
			pSRView->Release();
		});
	}

	/********************************************************************************
	*	--- Function:
	*	GetTexture(const char*)
	*	This function will return the shader resources view according to the name
	*
	*	--- Parameters:
	*	@ filename: the file name of the texture located in Assets folder
	*
	*	--- Return:
	*	@ void*: the pointer to D3D11 shader resources view
	********************************************************************************/
	void* GetTexture(const char* filename);

	/********************************************************************************
	*	--- Function:
	*	LoadTexture(const char*)
	*	This function will compile texture's shader resources view and insert it 
	*	into hash map
	*
	*	--- Parameters:
	*	@ filename: the file name of the texture located in Assets folder
	*
	*	--- Return:
	*	@ void*: the pointer to D3D11 shader resources view
	********************************************************************************/
	void LoadTexture(const char* filename);

	/********************************************************************************
	*	--- Static Function:
	*	GetInstance()
	*	This function will return the singleton instance of TextureManager
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ TextureManager*: the singleton instance
	********************************************************************************/
	static TextureManager* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new TextureManager();
		return m_pInstance;
	}

	/********************************************************************************
	*	--- Static Function:
	*	DestructandCleanUp()
	*	This function will delete the singleton instance
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	static void DestructandCleanUp()
	{
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

private:

	static TextureManager*								m_pInstance;	// Singleton instance
	MyHashMap<void*>									m_mapTexture;	// hash map of shader resources view

#ifdef D3D12
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT					m_SubresourceFootprintHolder[64]; // helper to store subresurce footprint to avoid allocate everytime
	UINT												m_RowCountHolder[64];
	UINT64												m_RowSizeHolder[64];
#endif
};

};
#endif // !TEXTUREMANAGER_H_
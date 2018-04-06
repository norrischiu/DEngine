#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <stdio.h>
#include <d3d12.h>
#include "Utilities\MyHashMap.h"
#include "GlobalInclude.h"

namespace DE
{

/*
*	CLASS: TextureManager
*	TextureManager is the resources manager for texture stored
*	as D3D shader resources view
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
	*	This constructor will release texture descriptor
	********************************************************************************/
	~TextureManager()
	{
		// TODO: release texture handle
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
	*	@ void*: the pointer to descriptor
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
	*	@ void
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

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT					m_SubresourceFootprintHolder[64]; // helper to store subresurce footprint to avoid allocate everytime
	UINT												m_RowCountHolder[64];
	UINT64												m_RowSizeHolder[64];
};

};
#endif // !TEXTUREMANAGER_H_
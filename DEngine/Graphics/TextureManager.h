// TextureManager.h:

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <stdio.h>
#include <d3d11.h>
#include "Utilities\MyHashMap.h"

namespace DE
{

class TextureManager
{
public:

	TextureManager()
		: m_mapTexture()
	{
	};

	// Return the texture according to the name
	void* GetTexture(const char* filename);

	// Insert texture data into map
	void LoadTexture(const char* filename);

	// Return singleton instance
	static TextureManager* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new TextureManager();
		return m_pInstance;
	}

	static void DestructandCleanUp()
	{
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~TextureManager()
	{
		m_mapTexture.ForEachItem([](void* item)
		{
			ID3D11ShaderResourceView* pSRView = (ID3D11ShaderResourceView*)item;
			pSRView->Release();
		});
	}

private:

	// Singleton instance
	static TextureManager*									m_pInstance;

	MyHashMap<void*>										m_mapTexture;

};

};
#endif // !TEXTUREMANAGER_H_
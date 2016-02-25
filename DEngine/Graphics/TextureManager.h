// TextureManager.h:

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <stdio.h>
#include <unordered_map>
#include <string>
#include <d3d11.h>

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
		std::unordered_map<std::string, void*>::iterator itr;
		for (itr = m_mapTexture.begin(); itr != m_mapTexture.end(); ++itr)
		{
			ID3D11ShaderResourceView* pSRView = (ID3D11ShaderResourceView*)itr->second;
			pSRView->Release();
		}
		m_mapTexture.clear();
	}

private:

	// Singleton instance
	static TextureManager*									m_pInstance;

	std::unordered_map<std::string, void*>					m_mapTexture;

};

#endif // !TEXTUREMANAGER_H_
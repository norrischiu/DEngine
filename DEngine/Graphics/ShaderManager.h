// ShaderManager.h:

#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <D3D11.h>
#include <stdio.h>
#include <d3d11shader.h>
#include "Utilities\MyHashMap.h"

namespace DE
{

class ShaderManager
{
public:

	ShaderManager()
		: m_mapShaders()
		, m_mapInputLayouts()
	{};

	// Return compiled shader stored in hash map
	void* GetShader(const char* filename, D3D11_SHADER_VERSION_TYPE type);

	// Compile and insert shader data into map
	void LoadShader(const char* filename, D3D11_SHADER_VERSION_TYPE type);

	// Create input layout of vertex shader
	void CreateInputLayout(ID3DBlob* VS, ID3D11InputLayout* &inputLayout);

	// Return input layout of vertex shader
	void* GetInputLayout(const char* filename);

	// Create stream out entry (shader output signature) of geometry shader
	D3D11_SO_DECLARATION_ENTRY* CreateStreamOutEntry(ID3DBlob* GS, unsigned int& entryNum);

	// Return singleton instance
	static ShaderManager* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new ShaderManager();
		return m_pInstance;
	}

	void DestructandCleanUp()
	{
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~ShaderManager()
	{
		m_mapShaders.ForEachItem([](void* item)
		{
			ID3D11DeviceChild* pS = (ID3D11DeviceChild*)item;
			pS->Release();
		});
		m_mapInputLayouts.ForEachItem([](void* item)
		{
			ID3D11InputLayout* pS = (ID3D11InputLayout*)item;
			pS->Release();
		});
	}

private:

	// Singleton instance
	static ShaderManager*									m_pInstance;

	MyHashMap<void*>										m_mapShaders;

	MyHashMap<ID3D11InputLayout*>							m_mapInputLayouts;
};

};
#endif // !SHADERMANAGER_H_
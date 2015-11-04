// ShaderManager.h:

#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_


#include <stdio.h>
#include <unordered_map>
#include <string>
#include "D3D11Renderer.h"
#include <d3d11shader.h>

class ShaderManager
{
public:

	ShaderManager()
		: m_mapShaders()
		, m_mapInputLayouts()
	{};

	//
	void* GetShader(const char* filename, D3D11_SHADER_VERSION_TYPE type);

	// Compile and insert shader data into map
	void LoadShader(const char* filename, D3D11_SHADER_VERSION_TYPE type);

	//
	void CreateInputLayout(ID3DBlob* VS, ID3D11InputLayout* &inputLayout);

	//
	void* GetInputLayout(const char* filename);

	// Return singleton instance
	static ShaderManager* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new ShaderManager();
		return m_pInstance;
	}

	static void DestructandCleanUp()
	{
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~ShaderManager()
	{
		std::unordered_map<std::string, void*>::iterator itr;
		for (itr = m_mapShaders.begin(); itr != m_mapShaders.end(); ++itr)
		{
			ID3D11DeviceChild* pS = (ID3D11DeviceChild*) itr->second;
			pS->Release();
		}
		m_mapShaders.clear();
		m_mapInputLayouts.clear();
	}

private:

	// Singleton instance
	static ShaderManager*									m_pInstance;

	std::unordered_map<std::string, void*>					m_mapShaders;
	
	std::unordered_map<std::string, ID3D11InputLayout*>		m_mapInputLayouts;
};

#endif // !SHADERMANAGER_H_
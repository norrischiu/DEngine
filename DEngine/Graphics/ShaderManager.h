#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <D3D11.h>
#include <stdio.h>
#include <d3d11shader.h>
#include "Utilities\MyHashMap.h"

namespace DE
{

/*
*	CLASS: ShaderManager
*	ShaderManager is the resources manager for compiled shader
*	as D3D11 shader  and also D3D11 input layout for vertex
*	shader
*/
class ShaderManager
{
public:

	/********************************************************************************
	*	--- Constructor:
	*	ShaderManager()
	*	This constructor will allocate default size of memory for hash map for
	*	shaders and input layout
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	ShaderManager()
		: m_mapShaders()
		, m_mapInputLayouts()
	{};

	/********************************************************************************
	*	--- Destructor:
	*	~ShaderManager()
	*	This constructor will free memory from hash map and release each D3D11 COM
	*	object
	********************************************************************************/
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

	/********************************************************************************
	*	--- Function:
	*	GetShader(const char*, D3D11_SHADER_VERSION_TYPE)
	*	This function will return compiled shader according to the given name
	*
	*	--- Parameters:
	*	@ filename: the file name of the shader located in Shaders folder
	*	@ type: the type of shader as defined in enum D3D11_SHADER_VERSION_TYPE
	*
	*	--- Return:
	*	@ void*: the pointer to a D3D11 shader
	********************************************************************************/
	void* GetShader(const char* filename, D3D11_SHADER_VERSION_TYPE type);

	/********************************************************************************
	*	--- Function:
	*	LoadShader(const char*, D3D11_SHADER_VERSION_TYPE)
	*	This function will compile a shader and insert it into hash map
	*
	*	--- Parameters:
	*	@ filename: the file name of the shader located in Shaders folder
	*	@ type: the type of shader as defined in enum D3D11_SHADER_VERSION_TYPE
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void LoadShader(const char* filename, D3D11_SHADER_VERSION_TYPE type);

	/********************************************************************************
	*	--- Function:
	*	CreateInputLayout(ID3DBlob*, ID3D11InputLayout*&)
	*	This function will create input layout of vertex shader
	*
	*	--- Parameters:
	*	@ VS: the pointer to compiled VS shader
	*	@ inputLayout: the resultant input layout from the vertex shader return
	*	as output
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void CreateInputLayout(ID3DBlob* VS, ID3D11InputLayout* &inputLayout);

	/********************************************************************************
	*	--- Function:
	*	GetInputLayout(const char*)
	*	This function will return the input layout associated to the given name of
	*	vertex shader
	*
	*	--- Parameters:
	*	@ filename: the file name of the vertex shader located in Shaders folder
	*
	*	--- Return:
	*	@ void*: the pointer to a D3D11 input layout
	********************************************************************************/
	void* GetInputLayout(const char* filename);

	/********************************************************************************
	*	--- Function:
	*	CreateStreamOutEntry(ID3DBlob*, unsigned int&)
	*	This function will create and return stream out entry of geometry shader
	*
	*	--- Parameters:
	*	@ GS: the pointer to compiled VS shader
	*	@ entryNum: the number of stream out entry (shader output signature), return
	*	as output
	*
	*	--- Return:
	*	@ D3D11_SO_DECLARATION_ENTRY*
	********************************************************************************/
	D3D11_SO_DECLARATION_ENTRY* CreateStreamOutEntry(ID3DBlob* GS, unsigned int& entryNum);

	/********************************************************************************
	*	--- Static Function:
	*	GetInstance()
	*	This function will return the singleton instance of ShaderManager
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ ShaderManager*: the singleton instance
	********************************************************************************/
	static ShaderManager* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new ShaderManager();
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
	void DestructandCleanUp()
	{
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

private:

	static ShaderManager*									m_pInstance;	// Singleton instance
	MyHashMap<void*>										m_mapShaders;	// hash map of D3D11 shader
	MyHashMap<ID3D11InputLayout*>							m_mapInputLayouts;	// hash map of input layout
};

};
#endif // !SHADERMANAGER_H_
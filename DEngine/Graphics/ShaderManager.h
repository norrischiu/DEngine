#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <D3D12.h>
#include <d3d12shader.h>
#include <stdio.h>
#include "GlobalInclude.h"
#include "Utilities\MyHashMap.h"

namespace DE
{

/*
*	CLASS: ShaderManager
*	ShaderManager is the resources manager for compiled shader
*	as D3D shader  and also D3D input layout for vertex
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
		, m_mapInputLayoutsCount()
	{};

	/********************************************************************************
	*	--- Destructor:
	*	~ShaderManager()
	*	This constructor will free memory from hash map and release each D3D
	*	resources
	********************************************************************************/
	~ShaderManager()
	{
		// TODO: free resources
	}

	/********************************************************************************
	*	--- Function:
	*	GetShader(const char*, D3D12_SHADER_VERSION_TYPE)
	*	This function will return compiled shader according to the given name
	*
	*	--- Parameters:
	*	@ filename: the file name of the shader located in Shaders folder
	*	@ type: the type of shader as defined in enum D3D12_SHADER_VERSION_TYPE
	*
	*	--- Return:
	*	@ void*: the pointer to a D3D shader
	********************************************************************************/
	void* GetShader(const char* filename, D3D12_SHADER_VERSION_TYPE type);

	/********************************************************************************
	*	--- Function:
	*	LoadShader(const char*, D3D12_SHADER_VERSION_TYPE)
	*	This function will compile a shader and insert it into hash map
	*
	*	--- Parameters:
	*	@ filename: the file name of the shader located in Shaders folder
	*	@ type: the type of shader as defined in enum D3D12_SHADER_VERSION_TYPE
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void LoadShader(const char* filename, D3D12_SHADER_VERSION_TYPE type);

	/********************************************************************************
	*	--- Function:
	*	CreateInputLayout(ID3DBlob*, void*&)
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
	void CreateInputLayout(ID3DBlob* VS, D3D12_INPUT_ELEMENT_DESC* &inputElementDesc, int &numElements);

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
	*	@ void*: the pointer to a D3D12 input layout
	********************************************************************************/
	D3D12_INPUT_ELEMENT_DESC* GetInputLayout(const char* filename);

	int GetInputLayoutCount(const char* filename)
	{
		return m_mapInputLayoutsCount[filename];
	}

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
	*	@ D3D12_SO_DECLARATION_ENTRY*
	********************************************************************************/
	D3D12_SO_DECLARATION_ENTRY* CreateStreamOutEntry(ID3DBlob* GS, unsigned int& entryNum);

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
	MyHashMap<void*>										m_mapShaders;	// hash map of hlsl shader
	MyHashMap<D3D12_INPUT_ELEMENT_DESC*>					m_mapInputLayouts;	// hash map of input layout
	MyHashMap<int>											m_mapInputLayoutsCount;	// hash map of input layout element num
};

};
#endif // !SHADERMANAGER_H_
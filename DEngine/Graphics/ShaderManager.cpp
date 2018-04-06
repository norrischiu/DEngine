#include "ShaderManager.h"
#include <d3dcompiler.h>
#include "Graphics\D3D12Renderer.h"
#include "GlobalInclude.h"

namespace DE
{

ShaderManager* ShaderManager::m_pInstance;

void* ShaderManager::GetShader(const char* filename, D3D12_SHADER_VERSION_TYPE type)
{
	std::string name(filename);
	if (!m_mapShaders.Contain(name.c_str()))
	{
		LoadShader(filename, type);
	}
	return m_mapShaders[filename];
}

void ShaderManager::LoadShader(const char* filename, D3D12_SHADER_VERSION_TYPE type)
{
	char* compileErrors;
	ID3DBlob* pRawData = nullptr;
	ID3DBlob* error;
	HRESULT hr;
	wchar_t* pName = new wchar_t[strlen(filename) + 1]; //TODO
	mbstowcs(pName, filename, strlen(filename) + 1);
	void* pShader = nullptr;
	D3D12_INPUT_ELEMENT_DESC* inputElmentDesc = nullptr;
	int numElements = 0;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	D3D12_SO_DECLARATION_ENTRY* pDecl = nullptr;

	switch (type)
	{
	case D3D12_SHVER_VERTEX_SHADER:
		hr = D3DCompileFromFile(pName, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &pRawData, &error);
		if (error) compileErrors = (char*)(error->GetBufferPointer());
		pShader = pRawData;
		CreateInputLayout(pRawData, inputElmentDesc, numElements);
		//inputLayoutDesc.pInputElementDescs = inputElmentDesc;
		//inputLayoutDesc.NumElements = numElements;
		m_mapInputLayouts.Add(filename, inputElmentDesc); 
		m_mapInputLayoutsCount.Add(filename, numElements);
		break;
	case D3D12_SHVER_PIXEL_SHADER:
		hr = D3DCompileFromFile(pName, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &pRawData, &error);
		if (error) compileErrors = (char*)(error->GetBufferPointer());
		pShader = pRawData;
		break;
	case D3D12_SHVER_GEOMETRY_SHADER:
		// TODO: D3D12 geometry shader
		break;
	case D3D12_SHVER_HULL_SHADER:
		// TODO: D3D12 hull shader
		break;
	case D3D12_SHVER_DOMAIN_SHADER:
		// TODO: D3D12 domain shader
		break;
	}

	m_mapShaders.Add(filename, pShader);
}

D3D12_SO_DECLARATION_ENTRY* ShaderManager::CreateStreamOutEntry(ID3DBlob* GS, unsigned int& entryNum)
{
	HRESULT hr;

	ID3D12ShaderReflection* pReflection;
	hr = D3DReflect(GS->GetBufferPointer(), GS->GetBufferSize(), IID_ID3D12ShaderReflection, (void**)&pReflection);
	assert(hr == S_OK);

	D3D12_SHADER_DESC shaderDesc;
	pReflection->GetDesc(&shaderDesc);
	unsigned int i = 0;
	int prevRegister = -1;
	unsigned int offset = 0;
	D3D12_SO_DECLARATION_ENTRY* data = new D3D12_SO_DECLARATION_ENTRY[shaderDesc.OutputParameters];
	for (; i < shaderDesc.OutputParameters; ++i)
	{
		D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflection->GetOutputParameterDesc(i, &paramDesc);
		D3D12_SO_DECLARATION_ENTRY SOEntryDesc;

		SOEntryDesc.SemanticName = paramDesc.SemanticName;
		SOEntryDesc.SemanticIndex = paramDesc.SemanticIndex;
		SOEntryDesc.StartComponent = 0;
		SOEntryDesc.Stream = 0;
		SOEntryDesc.OutputSlot = 0;

		if (prevRegister == paramDesc.Register)
		{
			offset += data[i - 1].ComponentCount;
		}
		else
		{
			offset = 0;
		}
		if (paramDesc.Mask == 1)
		{
			SOEntryDesc.ComponentCount = 1 - offset;
		}
		else if (paramDesc.Mask <= 3)
		{
			SOEntryDesc.ComponentCount = 2 - offset;
		}
		else if (paramDesc.Mask <= 7)
		{
			SOEntryDesc.ComponentCount = 3 - offset;
		}
		else if (paramDesc.Mask <= 15)
		{
			SOEntryDesc.ComponentCount = 4 - offset;
		}

		data[i] = SOEntryDesc;
		prevRegister = paramDesc.Register;
	}

	pReflection->Release();
	entryNum = i;
	return data;
}

void ShaderManager::CreateInputLayout(ID3DBlob* VS, D3D12_INPUT_ELEMENT_DESC* &inputElementDesc, int &numElements)
{
	MyArray<D3D12_INPUT_ELEMENT_DESC> inputElementDescArray(0, true);
	HRESULT hr;

	ID3D12ShaderReflection* pReflection;
	hr = D3DReflect(VS->GetBufferPointer(), VS->GetBufferSize(), IID_ID3D12ShaderReflection, (void**)&pReflection);
	assert(hr == S_OK);

	D3D12_SHADER_DESC shaderDesc;
	pReflection->GetDesc(&shaderDesc);
	unsigned int i = 0;
	for (; i < shaderDesc.InputParameters; ++i)
	{
		D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflection->GetInputParameterDesc(i, &paramDesc);
		D3D12_INPUT_ELEMENT_DESC inputElementDesc;
		MyArray<char> semanticName(strlen(paramDesc.SemanticName), true);
		for (int i = 0; i < strlen(paramDesc.SemanticName); ++i)
		{
			semanticName.Add(paramDesc.SemanticName[i]);
		}
		semanticName.Add('\0');
		inputElementDesc.SemanticName = semanticName.Raw();
		inputElementDesc.SemanticIndex = paramDesc.SemanticIndex;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		inputElementDesc.InputSlot = 0;
		inputElementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		inputElementDesc.InstanceDataStepRate = 0;

		inputElementDescArray.Add(inputElementDesc);
	}

	// Initialize input layout
	inputElementDesc = inputElementDescArray.Raw();
	numElements = i;

	pReflection->Release();
}

D3D12_INPUT_ELEMENT_DESC* ShaderManager::GetInputLayout(const char* filename)
{
	return m_mapInputLayouts[filename];
}


};
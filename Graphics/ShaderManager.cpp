#include "ShaderManager.h"
#include <d3dcompiler.h>
#include <vector>

ShaderManager* ShaderManager::m_pInstance;

void* ShaderManager::GetShader(const char* filename, D3D11_SHADER_VERSION_TYPE type)
{
	std::string name(filename);
	std::unordered_map<std::string, void*>::iterator result = m_mapShaders.find(name);
	if (result == m_mapShaders.end())
	{
		LoadShader(filename, type);
		return m_mapShaders[filename];
	}
	else return (*result).second;
}

void ShaderManager::LoadShader(const char* filename, D3D11_SHADER_VERSION_TYPE type)
{
	char* compileErrors;
	ID3DBlob* pRawData = nullptr;
	ID3DBlob* error;
	HRESULT hr;
	wchar_t* pName = new wchar_t[strlen(filename)+1]; //TODO
	mbstowcs(pName, filename, strlen(filename) + 1);
	void* pShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;

	switch (type)
	{
		case D3D11_SHVER_VERTEX_SHADER:
			ID3D11VertexShader* pVShader;
			pVShader = (ID3D11VertexShader*) pShader;
			hr = D3DCompileFromFile(pName, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &pRawData, &error);
			if (error) compileErrors = (char*)(error->GetBufferPointer());
			hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateVertexShader(pRawData->GetBufferPointer(), pRawData->GetBufferSize(), NULL, &pVShader);
			pShader = pVShader;

			inputLayout = nullptr;
			CreateInputLayout(pRawData, inputLayout);
			m_mapInputLayouts[filename] = inputLayout;

			break;
		case D3D11_SHVER_PIXEL_SHADER:
			ID3D11PixelShader* pPShader;
			pPShader = (ID3D11PixelShader*) pShader;
			hr = D3DCompileFromFile(pName, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &pRawData, &error);
			if (error) compileErrors = (char*)(error->GetBufferPointer());
			hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreatePixelShader(pRawData->GetBufferPointer(), pRawData->GetBufferSize(), NULL, &pPShader);
			pShader = pPShader;
			break;
		default:
			// TODO: not yet implemented
			assert(false);
	}

	m_mapShaders[filename] = pShader;
}

void ShaderManager::CreateInputLayout(ID3DBlob* VS, ID3D11InputLayout* &inputLayout)
{
	HRESULT hr;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescArray;

	ID3D11ShaderReflection* pReflection;
	hr = D3DReflect(VS->GetBufferPointer(), VS->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &pReflection);
	assert(hr == S_OK);

	D3D11_SHADER_DESC shaderDesc;
	pReflection->GetDesc(&shaderDesc);
	unsigned int i = 0;
	for (; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflection->GetInputParameterDesc(i, &paramDesc);
		D3D11_INPUT_ELEMENT_DESC inputElementDesc;

		inputElementDesc.SemanticName = paramDesc.SemanticName;
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
		inputElementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElementDesc.InstanceDataStepRate = 0;

		inputElementDescArray.push_back(inputElementDesc);
	}

	// Initialize input layout
	hr = D3D11Renderer::getInstance()->m_pD3D11Device->CreateInputLayout(&inputElementDescArray[0], i, VS->GetBufferPointer(), VS->GetBufferSize(), &inputLayout);
	assert(hr == S_OK);

	pReflection->Release();
}

void* ShaderManager::GetInputLayout(const char * filename)
{
	return m_mapInputLayouts[filename];
}


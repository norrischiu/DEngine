#include "ShaderManager.h"
#include <d3dcompiler.h>
#include <vector>
#include "Graphics\D3D11Renderer.h"

namespace DE
{

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
		wchar_t* pName = new wchar_t[strlen(filename) + 1]; //TODO
		mbstowcs(pName, filename, strlen(filename) + 1);
		void* pShader = nullptr;
		ID3D11InputLayout* inputLayout = nullptr;
		D3D11_SO_DECLARATION_ENTRY* pDecl = nullptr;

		switch (type)
		{
		case D3D11_SHVER_VERTEX_SHADER:
			ID3D11VertexShader* pVShader;
			pVShader = (ID3D11VertexShader*)pShader;
			hr = D3DCompileFromFile(pName, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &pRawData, &error);
			if (error) compileErrors = (char*)(error->GetBufferPointer());
			hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateVertexShader(pRawData->GetBufferPointer(), pRawData->GetBufferSize(), NULL, &pVShader);
			pShader = pVShader;

			inputLayout = nullptr;
			CreateInputLayout(pRawData, inputLayout);
			m_mapInputLayouts[filename] = inputLayout;

			break;
		case D3D11_SHVER_PIXEL_SHADER:
			ID3D11PixelShader* pPShader;
			pPShader = (ID3D11PixelShader*)pShader;
			hr = D3DCompileFromFile(pName, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &pRawData, &error);
			if (error) compileErrors = (char*)(error->GetBufferPointer());
			hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreatePixelShader(pRawData->GetBufferPointer(), pRawData->GetBufferSize(), NULL, &pPShader);
			pShader = pPShader;
			break;
		case D3D11_SHVER_GEOMETRY_SHADER:
			ID3D11GeometryShader* pGShader;
			pGShader = (ID3D11GeometryShader*)pShader;
			hr = D3DCompileFromFile(pName, NULL, NULL, "GS", "gs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &pRawData, &error);
			if (error) compileErrors = (char*)(error->GetBufferPointer());
			unsigned int entryNum;
			pDecl = CreateStreamOutEntry(pRawData, entryNum);
			if (pDecl != nullptr)
			{
				hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateGeometryShaderWithStreamOutput(pRawData->GetBufferPointer(), pRawData->GetBufferSize(), pDecl, entryNum, NULL, 0, 0, NULL, &pGShader);
			}
			else
			{
				hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateGeometryShader(pRawData->GetBufferPointer(), pRawData->GetBufferSize(), NULL, &pGShader);
			}
			pShader = pGShader;
			break;
		case D3D11_SHVER_HULL_SHADER:
			ID3D11HullShader* pHShader;
			pHShader = (ID3D11HullShader*)pShader;
			hr = D3DCompileFromFile(pName, NULL, NULL, "HS", "hs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &pRawData, &error);
			if (error) compileErrors = (char*)(error->GetBufferPointer());
			hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateHullShader(pRawData->GetBufferPointer(), pRawData->GetBufferSize(), NULL, &pHShader);
			pShader = pHShader;
			break;
		case D3D11_SHVER_DOMAIN_SHADER:
			ID3D11DomainShader* pDShader;
			pDShader = (ID3D11DomainShader*)pShader;
			hr = D3DCompileFromFile(pName, NULL, NULL, "DS", "ds_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &pRawData, &error);
			if (error) compileErrors = (char*)(error->GetBufferPointer());
			hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateDomainShader(pRawData->GetBufferPointer(), pRawData->GetBufferSize(), NULL, &pDShader);
			pShader = pDShader;
			break;
		}

		m_mapShaders[filename] = pShader;
	}

	D3D11_SO_DECLARATION_ENTRY* ShaderManager::CreateStreamOutEntry(ID3DBlob* GS, unsigned int& entryNum)
	{
		HRESULT hr;

		ID3D11ShaderReflection* pReflection;
		hr = D3DReflect(GS->GetBufferPointer(), GS->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflection);
		assert(hr == S_OK);

		D3D11_SHADER_DESC shaderDesc;
		pReflection->GetDesc(&shaderDesc);
		unsigned int i = 0;
		int prevRegister = -1;
		unsigned int offset = 0;
		D3D11_SO_DECLARATION_ENTRY* data = new D3D11_SO_DECLARATION_ENTRY[shaderDesc.OutputParameters];
		for (; i < shaderDesc.OutputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pReflection->GetOutputParameterDesc(i, &paramDesc);
			D3D11_SO_DECLARATION_ENTRY SOEntryDesc;

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

	void ShaderManager::CreateInputLayout(ID3DBlob* VS, ID3D11InputLayout* &inputLayout)
	{
		HRESULT hr;
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescArray;

		ID3D11ShaderReflection* pReflection;
		hr = D3DReflect(VS->GetBufferPointer(), VS->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflection);
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
		hr = D3D11Renderer::GetInstance()->m_pD3D11Device->CreateInputLayout(&inputElementDescArray[0], i, VS->GetBufferPointer(), VS->GetBufferSize(), &inputLayout);
		assert(hr == S_OK);

		pReflection->Release();
	}

	void* ShaderManager::GetInputLayout(const char * filename)
	{
		return m_mapInputLayouts[filename];
	}

};
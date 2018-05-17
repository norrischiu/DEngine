#include <string>
#include <stdio.h>
#include "Material.h"
#include "Graphics\Render\Texture.h"
#include "Graphics\D3D12Renderer.h"
#include "Graphics\MeshData.h"

namespace DE
{

void Material::ReadFromFile(const char * filename)
{
	std::string sFileName(filename);
	sFileName = "../Assets/" + sFileName + "_material.mate";
	FILE* pFile = fopen(sFileName.c_str(), "r");

	char c[256];
	float r, g, b;
	fscanf(pFile, "%s", &c);
	// read factors
	fscanf(pFile, "%f %f %f", &r, &g, &b);
	m_vEmissive = Vector3(r, g, b);
	fscanf(pFile, "%f %f %f", &r, &g, &b);
	m_vDiffuse = Vector3(r, g, b);
	fscanf(pFile, "%f %f %f", &r, &g, &b);
	m_vSpecular = Vector3(r, g, b);
	fscanf(pFile, "%f", &r);
	m_fShininess = r;
	// read textures
	int size;
	fscanf(pFile, "%i", &size);
	m_vTextures.Resize(size);
	for (int i = 0; i < size; ++i)
	{
		fscanf(pFile, "%s", &c);
		std::string str(c);
		if (!str.compare("DiffuseColor"))
		{
			m_TexFlag |= DIFFUSE;
		}
		else if (!str.compare("NormalMap") || !str.compare("Bump"))
		{
			m_TexFlag |= NORMAL;
		}
		else if (!str.compare("SpecularColor") || !str.compare("Specular"))
		{
			m_TexFlag |= SPECULAR;
		}
		fscanf(pFile, "%s", &c);
		Handle hTexture(sizeof(Texture));
		new (hTexture) Texture(Texture::SHADER_RESOURCES, 1, c, 10);
		m_vTextures.Add(hTexture);
	}

	fclose(pFile);
}

void Material::UseDefault()
{
	m_vEmissive = Vector3::Zero;
	m_vDiffuse = Vector3(1.0f, 1.0f, 1.0f);
	m_vSpecular = Vector3::Zero;
	m_fShininess = 1.0f;
}

void Material::BindToRenderer(Renderer * renderer)
{
	unsigned int size = m_vTextures.Size();
	if (size != 0)
	{
		UINT64 offset = renderer->m_CbvSrvUavHeapForShaderHandle.ptr - renderer->m_pCbvSrvUavHeapForShader->GetCPUDescriptorHandleForHeapStart().ptr;
		D3D12_GPU_DESCRIPTOR_HANDLE descriptorTable;
		descriptorTable.ptr = renderer->m_pCbvSrvUavHeapForShader->GetGPUDescriptorHandleForHeapStart().ptr + offset;
		for (int i = 0; i < size; i++)
		{
			if (m_vTextures[i].Raw() == nullptr)
			{
				// null descriptor
				D3D12_CPU_DESCRIPTOR_HANDLE nullDescriptor;
				D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc;
				SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				SRVDesc.Texture2D.MostDetailedMip = 0;
				SRVDesc.Texture2D.MipLevels = 0;
				SRVDesc.Texture2D.PlaneSlice = 0;
				renderer->m_pDevice->CreateShaderResourceView(nullptr, &SRVDesc, renderer->m_CbvSrvUavHeapForShaderHandle);
				renderer->m_CbvSrvUavHeapForShaderHandle.Offset(renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
			}
			else
			{
				D3D12_CPU_DESCRIPTOR_HANDLE srvCpuStart = (reinterpret_cast<Texture*>(m_vTextures[i].Raw()))->GetSRV();
				renderer->m_pDevice->CopyDescriptorsSimple(1, renderer->m_CbvSrvUavHeapForShaderHandle, srvCpuStart, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
				renderer->m_CbvSrvUavHeapForShaderHandle.Offset(renderer->m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
			}
		}
		renderer->m_pCommandList->SetGraphicsRootDescriptorTable(3, descriptorTable);
	}
}

};
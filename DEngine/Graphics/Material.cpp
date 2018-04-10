#include <string>
#include <stdio.h>
#include "Material.h"
#include "Graphics\Render\Texture.h"
#include "Graphics\D3D12Renderer.h"
#include "Graphics\MeshData.h"

namespace DE
{

void Material::ReadFromFile(const char * filename, int meshType)
{
	FILE* pFile = fopen(filename, "r");
	RenderPass* pass = new RenderPass;

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
		pass->AddTexture(hTexture);
	}

	fclose(pFile);

	// decide which technique to use
	switch (meshType)
	{
	case eMeshType::OUTLINE:
		pass->SetVertexShader("../DEngine/Shaders/VS_vertex1P.hlsl");
		break;
	case eMeshType::STANDARD_MESH:
		pass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1N1T1UV.hlsl");
		break;
	case eMeshType::SKELETAL_MESH:
		pass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1N1T1UV4J.hlsl");
		break;
	}
	if (m_TexFlag == (DIFFUSE | NORMAL | SPECULAR))
	{
		pass->SetPixelShader("../DEngine/Shaders/PS_vertex1P1N1T1UV_DiffuseSpecularBump_deferred.hlsl");
	}
	else if (m_TexFlag == (DIFFUSE | NORMAL))
	{
		pass->SetPixelShader("../DEngine/Shaders/PS_vertex1P1N1T1UV_DiffuseBump_deferred.hlsl");
	}
	else if (m_TexFlag == (DIFFUSE | SPECULAR))
	{
		pass->SetPixelShader("../DEngine/Shaders/PS_vertex1P1N1T1UV_DiffuseSpecular_deferred.hlsl");
	}
	else if (m_TexFlag == DIFFUSE)
	{
		pass->SetPixelShader("../DEngine/Shaders/PS_vertex1P1N1T1UV_Diffuse_deferred.hlsl");
	}
	else
	{
		pass->SetPixelShader(nullptr);
	}
	pass->SetBlendState(State::DEFAULT_BS);
	pass->SetRenderTargets(Renderer::GetInstance()->m_pRTV, 2);
	pass->SetDepthStencilView(Renderer::GetInstance()->m_depth);
	pass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);
	pass->ConstructPSO();
	m_pRenderPass = pass;
}

void Material::UseDefault()
{
	m_vEmissive = Vector3::Zero;
	m_vDiffuse = Vector3(1.0f, 1.0f, 1.0f);
	m_vSpecular = Vector3::Zero;
	m_fShininess = 1.0f;
}

};
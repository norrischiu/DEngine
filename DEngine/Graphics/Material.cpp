#include <string>
#include <stdio.h>
#include "Material.h"
#include "Graphics\Render\Texture.h"
#include "Graphics\D3D11Renderer.h"
#include "Graphics\MeshData.h"

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
		else if (!str.compare("NormalMap"))
		{
			m_TexFlag |= NORMAL;
		}
		fscanf(pFile, "%s", &c);
		pass->AddTexture(new Texture(Texture::SHADER_RESOURCES, 1, c));
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
	if (m_TexFlag & (DIFFUSE | NORMAL))
	{
		pass->SetPixelShader("../DEngine/Shaders/PS_vertex1P1N1T1UV_deferred.hlsl");
		pass->SetBlendState(State::NULL_STATE);
		pass->SetRenderTargets(D3D11Renderer::GetInstance()->m_pRTVArray, 2);
		pass->SetDepthStencilView(D3D11Renderer::GetInstance()->m_depth->GetDSV());
		pass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);
		m_pRenderTechnique->AddPass(pass);
	}
}

void Material::UseDefault()
{
	m_vEmissive = Vector3::Zero;
	m_vDiffuse = Vector3(1.0f, 1.0f, 1.0f);
	m_vSpecular = Vector3::Zero;
	m_fShininess = 1.0f;
}
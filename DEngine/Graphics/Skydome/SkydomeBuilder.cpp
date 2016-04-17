#include "SkydomeBuilder.h"
#include "../../Graphics/MeshComponent.h"
#include "../../Graphics/Scene/SceneGraph.h"
#include "../../Graphics/D3D11Renderer.h"
#include <fstream>

namespace DE
{

SkydomeBuilder* SkydomeBuilder::m_instance = nullptr;

SkydomeBuilder::SkydomeBuilder() : 
	m_vertices(nullptr),
	m_iNumVertices(0)
{
}


SkydomeBuilder::~SkydomeBuilder()
{
	if (!m_vertices) {
		delete[] m_vertices;
	}
}


SkydomeBuilder* SkydomeBuilder::getInstance()
{
	if (!m_instance) {
		m_instance = new SkydomeBuilder;
	}

	return m_instance;
}

bool SkydomeBuilder::LoadSkyDomeModel(const char* filename)
{
	std::string filepath = "../Assets/" + std::string(filename);

	std::ifstream fin;
	char input;

	// Open the model file.
	fin.open(filepath.c_str());

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_iNumVertices;

	// Create the model using the vertex count that was read in.
	m_vertices = new Vertex1P1N1D[m_iNumVertices];
	if (!m_vertices)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (int i = 0; i < m_iNumVertices; i++)
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		fin >> x >> y >> z;
		fin >> u >> v;
		fin >> nx >> ny >> nz;
	
		m_vertices[i].m_pos = Vector3(x, y, z);
		m_vertices[i].m_norm = Vector3(nx, ny, nz);
	}

	// Close the model file.
	fin.close();

	return true;
}

GameObject* SkydomeBuilder::CreateGameObject(const char* filename, const Vector3& apexColor, const Vector3& centerColor)
{
	GameObject* skydome = nullptr;

	DE::Matrix4 transform;
	transform.CreateScale(33.75f);

	if (LoadSkyDomeModel(filename))
	{
		Vertex1P1N1D* vertices = new Vertex1P1N1D[m_iNumVertices];

		const int iNumIndices = m_iNumVertices;
		unsigned int* indices = new unsigned int[iNumIndices];

		for (int i = 0; i < m_iNumVertices; i++)
		{
			vertices[i].m_pos = m_vertices[i].m_pos;
			vertices[i].m_norm = m_vertices[i].m_norm;
			vertices[i].m_diffuse = Lerp(centerColor, apexColor, max(0.0f, vertices[i].m_pos.GetY()));
			indices[i] = i;
		}

		MeshData* meshData = new MeshData(vertices, m_iNumVertices, indices, iNumIndices, sizeof(Vertex1P1N1D));
		Handle hMesh(sizeof(MeshComponent));
		new (hMesh) MeshComponent(meshData);
		MeshComponent* meshComponent = (MeshComponent*)hMesh.Raw();

		SceneGraph::GetInstance()->AddComponent(meshComponent);

		RenderPass* renderPass = new RenderPass;
		renderPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1N1D.hlsl");
		renderPass->SetPixelShader("../DEngine/Shaders/PS_diffuse.hlsl");
		renderPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderPass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
		renderPass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
		renderPass->SetRasterizerState(State::CULL_NONE_RS);
		meshComponent->m_pMeshData->m_Material.AddPassToTechnique(renderPass);

		skydome = new GameObject;
		skydome->AddComponent(meshComponent);

		delete[] vertices;
		delete[] indices;
	}

	return skydome;
}

};
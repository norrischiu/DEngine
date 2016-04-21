#include "Terrain.h"
#include "../D3D11Renderer.h"
#include "../MeshComponent.h"
#include "../Scene/SceneGraph.h"
#include "../VertexFormat.h"



namespace DE
{
Terrain::Terrain(const InitInfo initInfo, std::vector<float> heightMap)
	: m_initInfo(initInfo), m_HeightMap(heightMap)
{
}

Terrain::~Terrain()
{
}

float Terrain::GetWidth() const
{
	return (m_initInfo.HeightmapWidth - 1) * m_initInfo.CellSpacing;
}

float Terrain::GetDepth() const
{
	// Total terrain depth.
	return (m_initInfo.HeightmapHeight - 1) * m_initInfo.CellSpacing;
}

float Terrain::GetHeight(float x, float z) const
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f * GetWidth()) / m_initInfo.CellSpacing;
	float d = (z + 0.5f * GetDepth()) / m_initInfo.CellSpacing;

	// Get the row and column we are in.
	int row = (int) floorf(d);
	int col = (int) floorf(c);

	// Grab the heights of the cell we are in.
	float A = m_HeightMap[row * m_initInfo.HeightmapWidth + col];
	float B = m_HeightMap[row * m_initInfo.HeightmapWidth + col + 1];
	float C = m_HeightMap[(row + 1) * m_initInfo.HeightmapWidth + col];
	float D = m_HeightMap[(row + 1) * m_initInfo.HeightmapWidth + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	float height = 0.0f;

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		height = A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		height = D + (1.0f - s)*uy + (1.0f - t)*vy;
	}

	return height;
}

std::vector<float2> Terrain::CalcAllPatchBoundsY()
{
	std::vector<float2> patchBoundsY(m_initInfo.HeightmapHeight *  (m_initInfo.HeightmapWidth / m_initInfo.CellsPerPatch));
	int patch_id = 0;

	for (int j = 0; j < m_initInfo.HeightmapHeight-8; j += 8)
	{
		for (int i = 0; i < m_initInfo.HeightmapWidth-8; i += 8)
		{
			patchBoundsY[patch_id] = CalcPatchBoundsY(j, i);
			patch_id++;
		}
	}

	return patchBoundsY;
}

float2 Terrain::CalcPatchBoundsY(const int start_j, const int start_i)
{
	float minY = (std::numeric_limits<float>::max)();
	float maxY = -(std::numeric_limits<float>::min)();


	for (int j = start_j; j < start_j + 8; j++)
	{
		for (int i = start_i; i < start_i + 8; i++)
		{
			const int index = j * m_initInfo.HeightmapWidth + i;
			minY = min(minY, m_HeightMap[index]);
			maxY = max(maxY, m_HeightMap[index]);
		}
	}

	const float2 boundY(minY, maxY);
	return boundY;
}

std::vector<float2> Terrain::calculateTextureCoordiate()
{
	std::vector<float2> texture_coordinate(m_HeightMap.size());

	const int texture_repeat = 0;
	const int incrementCount = m_initInfo.HeightmapWidth / (texture_repeat + 1);
	const float incrementValue = (float)(texture_repeat + 1) / (float)m_initInfo.HeightmapWidth;

	// Initialize the tu and tv coordinate values.
	float tuCoordinate = 0.0f;
	float tvCoordinate = 0.0f;

	// Initialize the tu and tv coordinate indexes.
	int tuCount = 0;
	int tvCount = 0;

	for (int j = 0; j < m_initInfo.HeightmapHeight; j++)
	{
		for (int i = 0; i < m_initInfo.HeightmapWidth; i++)
		{
			texture_coordinate[(m_initInfo.HeightmapWidth * j) + i].x = tuCoordinate;
			texture_coordinate[(m_initInfo.HeightmapWidth * j) + i].y = tvCoordinate;

			tuCoordinate += incrementValue;
			tuCount++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if (tuCount == incrementCount)
			{
				tuCoordinate = 0.0f;
				tuCount = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate += incrementValue;
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if (tvCount == incrementCount)
		{
			tvCoordinate = 0.0f;
			tvCount = 0;
		}
	}

	return texture_coordinate;
}

GameObject* Terrain::CreateGameObject(const char* diffuseTxt_filename, const char* normalTxt_filename)
{
	std::vector<float2> texture_coordinate = calculateTextureCoordiate();
	std::vector<float2> boundsY = CalcAllPatchBoundsY();

	const int iNumVertices = ((m_initInfo.HeightmapHeight - 8) / 8) * ((m_initInfo.HeightmapWidth - 8) / 8) * 4;
	const int iNumIndices = iNumVertices;
	VertexTerrain* vertices = new VertexTerrain[iNumVertices];
	unsigned int* indices = new unsigned int[iNumIndices];

	// Translate the terrain, so that the mid-point of terrain is at (0, 0, 0)
	Matrix4 translate;
	translate.CreateTranslation(Vector3(-GetWidth() / 2.0f, 0.0f, -GetDepth() / 2.0f));

	// Initialize the index to the vertex buffer.
	int indexCounter = 0;
	int patch_id = 0;

	// Load the vertex and index array with the terrain data.
	for (int j = 0; j < m_initInfo.HeightmapHeight - 8; j += 8)
	{
		for (int i = 0; i < m_initInfo.HeightmapWidth - 8; i += 8)
		{
			const int index_bl = m_initInfo.HeightmapWidth * j + i;
			const int index_br = m_initInfo.HeightmapWidth * j + (i + 8);
			const int index_ul = m_initInfo.HeightmapWidth * (j + 8) + i;
			const int index_ur = m_initInfo.HeightmapWidth * (j + 8) + (i + 8);

			const float2 bl_uv(
				texture_coordinate[index_bl].x, texture_coordinate[index_bl].y
			);
			const float2 br_uv(
				(texture_coordinate[index_br].x == 0.0f ? 1.0f : texture_coordinate[index_br].x),
				texture_coordinate[index_br].y
			);
			const float2 ul_uv(
				texture_coordinate[index_ul].x,
				(texture_coordinate[index_ul].y == 1.0f ? 0.0f : texture_coordinate[index_ul].y)
			);
			const float2 ur_uv(
				(texture_coordinate[index_ur].x == 0.0f ? 1.0f : texture_coordinate[index_ur].x),
				(texture_coordinate[index_ur].y == 1.0f ? 0.0f : texture_coordinate[index_ur].y)
			);

			Vector3 bl(i * m_initInfo.CellSpacing, m_HeightMap[index_bl] * m_initInfo.CellSpacing, j * m_initInfo.CellSpacing);
			Vector3 br((i + 8) * m_initInfo.CellSpacing, m_HeightMap[index_br] * m_initInfo.CellSpacing, j * m_initInfo.CellSpacing);
			Vector3 ul(i * m_initInfo.CellSpacing, m_HeightMap[index_ul] * m_initInfo.CellSpacing, (j + 8) * m_initInfo.CellSpacing);
			Vector3 ur((i + 8) * m_initInfo.CellSpacing, m_HeightMap[index_ur] * m_initInfo.CellSpacing, (j + 8) * m_initInfo.CellSpacing);

			bl.Transform(translate);
			br.Transform(translate);
			ul.Transform(translate);
			ur.Transform(translate);

			// bottom left
			{
				vertices[indexCounter].m_pos = bl;
				vertices[indexCounter].m_UV[0] = bl_uv.x;
				vertices[indexCounter].m_UV[1] = bl_uv.y;
				vertices[indexCounter].m_boundsY[0] = boundsY[patch_id].x;
				vertices[indexCounter].m_boundsY[1] = boundsY[patch_id].y;
				indices[indexCounter] = indexCounter;
				indexCounter++;
			}

			// bottom right
			{
				vertices[indexCounter].m_pos = br;
				vertices[indexCounter].m_UV[0] = br_uv.x;
				vertices[indexCounter].m_UV[1] = br_uv.y;
				vertices[indexCounter].m_boundsY[0] = boundsY[patch_id].x;
				vertices[indexCounter].m_boundsY[1] = boundsY[patch_id].y;
				indices[indexCounter] = indexCounter;
				indexCounter++;
			}

			// upper left
			{
				vertices[indexCounter].m_pos = ul;
				vertices[indexCounter].m_UV[0] = ul_uv.x;
				vertices[indexCounter].m_UV[1] = ul_uv.y;
				vertices[indexCounter].m_boundsY[0] = boundsY[patch_id].x;
				vertices[indexCounter].m_boundsY[1] = boundsY[patch_id].y;
				indices[indexCounter] = indexCounter;
				indexCounter++;
			}

			// upper right
			{
				vertices[indexCounter].m_pos = ur;
				vertices[indexCounter].m_UV[0] = ur_uv.x;
				vertices[indexCounter].m_UV[1] = ur_uv.y;
				vertices[indexCounter].m_boundsY[0] = boundsY[patch_id].x;
				vertices[indexCounter].m_boundsY[1] = boundsY[patch_id].y;
				indices[indexCounter] = indexCounter;
				indexCounter++;
			}

			patch_id++;
		}
	}
	

	std::string diffuseTxt_filepath = std::string("../Assets/") + diffuseTxt_filename + ".dds";
	std::string normalTxt_filepath = std::string("../Assets/") + normalTxt_filename + ".dds";

	MeshData* meshData = new MeshData(vertices, iNumVertices, indices, iNumIndices, sizeof(VertexTerrain));
	meshData->SetBoundingBox(AABB(Vector3(0, 0, 0), Vector3(m_initInfo.HeightmapHeight, 0, m_initInfo.HeightmapWidth)));
	Handle hMeshComp(sizeof(MeshComponent));
	new (hMeshComp) MeshComponent(meshData);
	SceneGraph::GetInstance()->AddComponent((MeshComponent*) hMeshComp.Raw());

	RenderPass* renderPass = new RenderPass;
	renderPass->SetVertexShader("../DEngine/Shaders/VS_terrain.hlsl");
	renderPass->SetHullShader("../DEngine/Shaders/HS_terrain.hlsl");
	renderPass->SetDomainShader("../DEngine/Shaders/DS_terrain.hlsl");
	renderPass->SetPixelShader("../DEngine/Shaders/PS_terrain.hlsl");
	Handle hTexture1(sizeof(Texture));
	new (hTexture1) Texture(Texture::SHADER_RESOURCES, 1, diffuseTxt_filepath.c_str());
	Handle hTexture2(sizeof(Texture));
	new (hTexture2) Texture(Texture::SHADER_RESOURCES, 1, normalTxt_filepath.c_str());
	Handle hTexture3(sizeof(Texture));
	new (hTexture3) Texture(m_HeightMap, m_initInfo.HeightmapWidth, m_initInfo.HeightmapWidth);
	renderPass->AddTexture(hTexture1);
	renderPass->AddTexture(hTexture2);
	renderPass->AddTexture(hTexture3);
	renderPass->SetTopology(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	renderPass->SetBlendState(State::NULL_STATE);
	renderPass->SetRenderTargets(D3D11Renderer::GetInstance()->m_pRTVArray, 2);
	//renderPass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
	renderPass->SetDepthStencilView(D3D11Renderer::GetInstance()->m_depth->GetDSV());
	renderPass->SetDepthStencilState(State::DEFAULT_DEPTH_STENCIL_DSS);
	renderPass->SetRasterizerState(State::CULL_BACK_RS);
	((MeshComponent*) hMeshComp.Raw())->m_pMeshData->m_Material.AddPassToTechnique(renderPass);

	GameObject* terrain = new GameObject;
	terrain->AddComponent((Component*) hMeshComp.Raw());

	delete[] vertices;
	delete[] indices;

	return terrain;
}

};
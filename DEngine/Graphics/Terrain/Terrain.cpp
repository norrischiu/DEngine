#include "Terrain.h"
#include "../D3D11Renderer.h"
#include "../MeshComponent.h"
#include "../Scene/SceneGraph.h"
#include "../VertexFormat.h"



namespace DE
{

	Terrain* Terrain::m_pInstance;

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
		float c = (x + 0.5f*GetWidth()) / m_initInfo.CellSpacing;
		float d = (z - 0.5f*GetDepth()) / -m_initInfo.CellSpacing;

		// Get the row and column we are in.
		int row = (int)floorf(d);
		int col = (int)floorf(c);

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
		std::vector<float2> patchBoundsY(m_HeightMap.size() / m_initInfo.CellsPerPatch);

		for (int j = 0; j < m_initInfo.HeightmapHeight; j++)
		{
			for (int i = 0; i < m_initInfo.HeightmapWidth; i += m_initInfo.CellsPerPatch)
			{
				const int patch_id = j + (i / m_initInfo.CellsPerPatch);
				patchBoundsY[patch_id] = CalcPatchBoundsY(patch_id);
			}
		}

		return patchBoundsY;
	}

	float2 Terrain::CalcPatchBoundsY(const int patch_id)
	{
		float minY = (std::numeric_limits<float>::max)();
		float maxY = -(std::numeric_limits<float>::min)();

		const int start = patch_id *  m_initInfo.CellsPerPatch;
		const int end = (patch_id + 1) *  m_initInfo.CellsPerPatch;

		for (int i = start; i < end; i++)
		{
			minY = min(minY, m_HeightMap[i]);
			maxY = max(maxY, m_HeightMap[i]);
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

	std::vector<Vector3> Terrain::calculateNormal()
	{
		std::vector<Vector3> normal(m_HeightMap.size());

		// Translate the terrain, so that the mid-point of terrain is at (0, 0, 0)
		Matrix4 translate;
		translate.CreateTranslation(Vector3(-GetWidth() / 2.0f, 0.0f, -GetDepth() / 2.0f));

		// Go through all the faces in the mesh and calculate their normals.
		for (int j = 0; j < m_initInfo.HeightmapHeight - 1; j++)
		{
			for (int i = 0; i < m_initInfo.HeightmapWidth - 1; i++)
			{
				const int index[4] = {
					(m_initInfo.HeightmapWidth - 1) * j + i,
					m_initInfo.HeightmapWidth * j + i,
					m_initInfo.HeightmapWidth * j + (i + 1),
					m_initInfo.HeightmapWidth * (j + 1) + i
				};

				// Get three vertices from the face.
				Vector3 vertices[3] = {
					Vector3(i * m_initInfo.CellSpacing, m_HeightMap[index[1]], j * m_initInfo.CellSpacing),
					Vector3((i + 1) * m_initInfo.CellSpacing, m_HeightMap[index[2]], j * m_initInfo.CellSpacing),
					Vector3(i * m_initInfo.CellSpacing, m_HeightMap[index[3]], (j + 1) * m_initInfo.CellSpacing)
				};
				vertices[0].Transform(translate);
				vertices[1].Transform(translate);
				vertices[2].Transform(translate);

				const Vector3 vectors[2] = {
					Vector3(vertices[0].GetX() - vertices[2].GetX(), vertices[0].GetY() - vertices[2].GetY(), vertices[0].GetZ() - vertices[2].GetZ()),
					Vector3(vertices[2].GetX() - vertices[1].GetX(), vertices[2].GetY() - vertices[1].GetY(), vertices[2].GetZ() - vertices[1].GetZ())
				};

				// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
				normal[index[0]] = Cross(vectors[0], vectors[1]);
			}
		}

		// Now go through all the vertices and take an average of each face normal 	
		// that the vertex touches to get the averaged normal for that vertex.
		for (int j = 0; j < m_initInfo.HeightmapHeight; j++)
		{
			for (int i = 0; i < m_initInfo.HeightmapWidth; i++)
			{
				// Initialize the sum.
				Vector3 sum(0.0f, 0.0f, 0.0f);

				// Initialize the count.
				int count = 0;

				// Bottom left face.
				if (((i - 1) >= 0) && ((j - 1) >= 0))
				{
					const int index = (m_initInfo.HeightmapWidth - 1) * (j - 1) + (i - 1);

					sum = sum + normal[index];
					count++;
				}

				// Bottom right face.
				if ((i < (m_initInfo.HeightmapWidth - 1)) && ((j - 1) >= 0))
				{
					const int index = (m_initInfo.HeightmapWidth - 1) * (j - 1) + i;

					sum = sum + normal[index];
					count++;
				}

				// Upper left face.
				if (((i - 1) >= 0) && (j < (m_initInfo.HeightmapHeight - 1)))
				{
					const int index = (m_initInfo.HeightmapWidth - 1) * j + (i - 1);

					sum = sum + normal[index];
					count++;
				}

				// Upper right face.
				if ((i < (m_initInfo.HeightmapWidth - 1)) && (j < (m_initInfo.HeightmapHeight - 1)))
				{
					const int index = (j * (m_initInfo.HeightmapWidth - 1)) + i;

					sum = sum + normal[index];
					count++;
				}

				// Take the average of the faces touching this vertex.
				sum.SetX(sum.GetX() / (float)count);
				sum.SetY(sum.GetY() / (float)count);
				sum.SetZ(sum.GetZ() / (float)count);

				// Get an index to the vertex location in the height map array.
				const int index = m_initInfo.HeightmapWidth * j + i;

				// Normalize the final shared normal for this vertex and store it in the height map array.
				normal[index] = sum;
				normal[index].SetW(1.0f);
			}
		}

		return normal;
	}

	const Vector3 Terrain::calculateTangent(const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3, const float2 vertex1_uv, const float2 vertex2_uv, const float2 vertex3_uv)
	{
		const Vector3 vectors[2] = {
			Vector3(
			vertex2.GetX() - vertex1.GetX(),
				vertex2.GetY() - vertex1.GetY(),
				vertex2.GetZ() - vertex1.GetZ()
				),
			Vector3(
				vertex3.GetX() - vertex1.GetX(),
				vertex3.GetY() - vertex1.GetY(),
				vertex3.GetZ() - vertex1.GetZ()
				)
		};

		const float2 uvVectors[2] = {
			float2(vertex2_uv.x - vertex1_uv.x, vertex3_uv.y - vertex1_uv.y),
			float2(vertex2_uv.x - vertex1_uv.x, vertex3_uv.y - vertex1_uv.y)
		};

		// Calculate the denominator of the tangent/binormal equation.
		const float den = 1.0f; // 1.0f / (uvVectors[0].x * uvVectors[1].y - uvVectors[0].y * uvVectors[1].x);

		Vector3 tangent = Vector3(
			(uvVectors[1].y * vectors[0].GetX() - uvVectors[1].x * vectors[1].GetX()) * den,
			(uvVectors[1].y * vectors[0].GetY() - uvVectors[1].x * vectors[1].GetY()) * den,
			(uvVectors[1].y * vectors[0].GetZ() - uvVectors[1].x * vectors[1].GetZ()) * den
			);

		return tangent;
	}

	GameObject* Terrain::CreateGameObject(const char* diffuseTxt_filename, const char* normalTxt_filename, const char* heightTxt_filemane)
	{
		m_numPatchRows = m_initInfo.HeightmapHeight / 8 + 1;
		m_numPatchCols = m_initInfo.HeightmapWidth / 8 + 1;

		std::vector<float2> texture_coordinate = calculateTextureCoordiate();
		std::vector<Vector3> normal = calculateNormal();
		std::vector<float2> boundsY = CalcAllPatchBoundsY();

		//const int iNumVertices = (m_initInfo.HeightmapWidth - 1) * (m_initInfo.HeightmapHeight - 1) * 6;
		const int iNumVertices = m_initInfo.HeightmapWidth / 8 * (m_initInfo.HeightmapHeight) / 8 * 4;
		const int iNumIndices = iNumVertices;
		Vertex1P1N1T1B1UV* vertices = new Vertex1P1N1T1B1UV[iNumVertices];
		unsigned int* indices = new unsigned int[iNumIndices];

		// Translate the terrain, so that the mid-point of terrain is at (0, 0, 0)
		Matrix4 translate;
		translate.CreateTranslation(Vector3(-GetWidth() / 2.0f, 0.0f, -GetDepth() / 2.0f));

		// Initialize the index to the vertex buffer.
		int indexCounter = 0;
		float du = 1.0f / (m_numPatchCols - 1);
		float dv = 1.0f / (m_numPatchRows - 1);

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

				Vector3 bl(i * m_initInfo.CellSpacing, 0, j* m_initInfo.CellSpacing);
				Vector3 br((i + 8)* m_initInfo.CellSpacing, 0, j* m_initInfo.CellSpacing);
				Vector3 ul(i* m_initInfo.CellSpacing, 0, (j + 8)* m_initInfo.CellSpacing);
				Vector3 ur((i + 8)* m_initInfo.CellSpacing, 0, (j + 8)* m_initInfo.CellSpacing);

				const Vector3 tangent_lTri = calculateTangent(bl, ul, ur, bl_uv, ul_uv, ur_uv);
				const Vector3 tangent_rTri = calculateTangent(bl, ul, ur, bl_uv, ul_uv, ur_uv);

				const int patch_id = j + (i / m_initInfo.CellsPerPatch);
				// bottom left
				{
					vertices[indexCounter].m_pos = bl;
					vertices[indexCounter].m_norm = normal[index_bl];
					vertices[indexCounter].m_tangent = tangent_lTri;
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
					vertices[indexCounter].m_norm = normal[index_br];
					vertices[indexCounter].m_tangent = tangent_rTri;
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
					vertices[indexCounter].m_norm = normal[index_ul];
					vertices[indexCounter].m_tangent = tangent_lTri;
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
					vertices[indexCounter].m_norm = normal[index_br];
					vertices[indexCounter].m_tangent = tangent_rTri;
					vertices[indexCounter].m_UV[0] = ur_uv.x;
					vertices[indexCounter].m_UV[1] = ur_uv.y;
					vertices[indexCounter].m_boundsY[0] = boundsY[patch_id].x;
					vertices[indexCounter].m_boundsY[1] = boundsY[patch_id].y;
					indices[indexCounter] = indexCounter;
					indexCounter++;
				}
			}
		}

		std::string diffuseTxt_filepath = std::string("../Assets/") + diffuseTxt_filename;
		std::string normalTxt_filepath = std::string("../Assets/") + normalTxt_filename;
		std::string heightTxt_filepath = std::string("../Assets/") + heightTxt_filemane;

		MeshData* meshData = new MeshData(vertices, iNumVertices, indices, iNumIndices, sizeof(Vertex1P1N1T1B1UV));
		Handle hMeshComp(sizeof(MeshComponent));
		new (hMeshComp) MeshComponent(meshData);
		SceneGraph::GetInstance()->AddComponent((MeshComponent*) hMeshComp.Raw());

		RenderPass* renderPass = new RenderPass;
		renderPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1N1T1B1UV.hlsl");
		renderPass->SetHullShader("../DEngine/Shaders/HS_terrain.hlsl");
		renderPass->SetDomainShader("../DEngine/Shaders/DS_terrain.hlsl");
		renderPass->SetPixelShader("../DEngine/Shaders/PS_vertex1P1N1T1B1UV_deferred.hlsl");
		Handle hTexture1(sizeof(Texture));
		new (hTexture1) Texture(Texture::SHADER_RESOURCES, 1, diffuseTxt_filepath.c_str());
		Handle hTexture2(sizeof(Texture));
		new (hTexture2) Texture(Texture::SHADER_RESOURCES, 1, normalTxt_filepath.c_str());
		Handle hTexture3(sizeof(Texture));
		new (hTexture3) Texture(Texture::SHADER_RESOURCES, 1, heightTxt_filepath.c_str());
		renderPass->AddTexture(hTexture1);
		renderPass->AddTexture(hTexture2);
		renderPass->AddTexture(hTexture3);
		renderPass->SetTopology(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		renderPass->SetBlendState(State::NULL_STATE);
		renderPass->SetRenderTargets(D3D11Renderer::GetInstance()->m_pRTVArray, 2);
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
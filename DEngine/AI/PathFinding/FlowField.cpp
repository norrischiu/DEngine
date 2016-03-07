#include "FlowField.h"
#include "../../Graphics/MeshComponent.h"
#include "../../Graphics/D3D11Renderer.h"
#include "../../Graphics/Scene/SceneGraph.h"

namespace DE
{

float getDirectionInRadian(const Vector3& direction)
{
	return atan2(direction.GetZ(), direction.GetX());
}

FlowField::FlowField(std::vector<std::vector<std::vector<Cell>>> flowField, const Vector3& destination)
	: m_flowField(flowField), m_destination(destination)
{
}

FlowField::~FlowField()
{
}

void FlowField::print()
{
	const int iNumVertices = (getFlowFieldWidth() + 1) * 2 + (getFlowFieldDepth() + 1) * 2 + 4 * getFlowFieldWidth() * getFlowFieldDepth();
	const int iNumIndices = (getFlowFieldWidth() + 1) * 2 + (getFlowFieldDepth() + 1) * 2 + 6 * getFlowFieldWidth() * getFlowFieldDepth();
	Vertex1P* vertices = new Vertex1P[iNumVertices];
	unsigned int* indices = new unsigned int[iNumIndices];

	int index_ver = 0;
	int index_in = 0;
	for (int i = 0; i <= getFlowFieldDepth(); i++)
	{
		vertices[index_ver++].m_pos = Vector3(0.0f, 0.0f, i * 1.0f);
		vertices[index_ver++].m_pos = Vector3(getFlowFieldWidth() * 1.0f, 0.0f, i * 1.0f);
		indices[index_in] = index_in++;
		indices[index_in] = index_in++;
	}

	for (int i = 0; i <= getFlowFieldWidth(); i++)
	{
		vertices[index_ver++].m_pos = Vector3(i * 1.0f, 0.0f, 0.0f);
		vertices[index_ver++].m_pos = Vector3(i * 1.0f, 0.0f, getFlowFieldDepth() * 1.0f);
		indices[index_in] = index_in++;
		indices[index_in] = index_in++;
	}

	Vector3 dest = m_destination;
	Matrix4 transform;
	transform.CreateTranslation(Vector3(getFlowFieldWidth() / 2.0f, 0.0f, getFlowFieldDepth() / 2.0f));
	dest.Transform(transform);

	for (int i = 0; i < getFlowFieldWidth(); i++)
	{
		//for (int j = 0; j < getFlowFieldHeight(); j++)
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < getFlowFieldDepth(); k++)
			{
				if ((dest.GetX() - i - 1) + (dest.GetZ() - k - 1) < std::numeric_limits<float>::epsilon()) {
					continue;
				}

				Matrix4 translate;
				translate.CreateTranslation(Vector3(i + 0.5f, j, k + 0.5f));

				if (m_flowField[i][j][k].isMovable)
				{
					Matrix4 rotate;
					rotate.CreateRotationY(-getDirectionInRadian(m_flowField[i][j][k].direction));

					Vector3 arrow[4] = {
						Vector3(-0.3f, 0.0f, 0.0f),
						Vector3(0.3f, 0.0f, 0.0f),
						Vector3(0.15f, 0.0f, 0.15f),
						Vector3(0.15f, 0.0f, -0.15f)
					};

					for (int l = 0; l < 4; l++)
					{
						arrow[l].Transform(rotate);
						arrow[l].Transform(translate);
						vertices[index_ver++].m_pos = arrow[l];
					}

					indices[index_in++] = index_ver -4 + 0;
					indices[index_in++] = index_ver -4 + 1;
					indices[index_in++] = index_ver -4 + 2;
					indices[index_in++] = index_ver -4 + 1;
					indices[index_in++] = index_ver -4 + 3;
					indices[index_in++] = index_ver -4 + 1;
				}
				else
				{
					Vector3 cross[4] = {
						Vector3(-0.3f, 0.0f, 0.3f),
						Vector3(0.3f, 0.0f, -0.3f),
						Vector3(-0.3f, 0.0f, -0.3f),
						Vector3(0.3f, 0.0f, 0.3f)
					};

					for (int l = 0; l < 4; l++)
					{
						cross[l].Transform(translate);
						vertices[index_ver].m_pos = cross[l];
						indices[index_in++] = index_ver++;
					}
				}
			}
		}
	}

	MeshComponent* meshComponent = new MeshComponent(
		new MeshData(vertices, iNumVertices, indices, iNumIndices, sizeof(Vertex1P))
	);

	RenderPass* renderPass = new RenderPass;
	renderPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P.hlsl");
	renderPass->SetPixelShader("../DEngine/Shaders/PS_red.hlsl");
	renderPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	meshComponent->m_pMeshData->m_Material.AddPassToTechnique(renderPass);
	renderPass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
	renderPass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
	renderPass->SetRasterizerState(State::CULL_NONE_RS);
	Matrix4 translate;
	translate.CreateTranslation(Vector3(-getFlowFieldWidth() / 2.0f, 0.0f, -getFlowFieldDepth() / 2.0f));
	meshComponent->m_pTransform->Multiply(translate);
	SceneGraph::GetInstance()->ADD_DEBUG_DRAWING(meshComponent);
}

int FlowField::getFlowFieldWidth()
{
	return m_flowField.size();
}

int FlowField::getFlowFieldHeight()
{
	return m_flowField[0].size();
}

int FlowField::getFlowFieldDepth()
{
	return m_flowField[0][0].size();
}

bool FlowField::isPositionMovable(const Vector3& position)
{
	Matrix4 transform;
	transform.CreateTranslation(Vector3(getFlowFieldWidth() / 2.0f, 0.0f, getFlowFieldDepth() / 2.0f));
	Vector3 pos = position;
	pos.Transform(transform);

	const int x = abs(pos.GetX());
	const int y = abs(pos.GetY());
	const int z = abs(pos.GetZ());

	if (
		(x >= 0 && x <= getFlowFieldWidth() - 1) &&
		(y >= 0 && y <= getFlowFieldHeight() - 1) &&
		(z >= 0 && z <= getFlowFieldDepth() - 1)
	) {
		return m_flowField[x][y][z].isMovable;
	}

	return false;
}

const Vector3 FlowField::getDirection(const Vector3& position)
{
	Matrix4 transform;
	transform.CreateTranslation(Vector3(getFlowFieldWidth() / 2.0f, 0.0f, getFlowFieldDepth() / 2.0f));
	Vector3 pos = position;
	pos.Transform(transform);

	const int x = pos.GetX();
	const int y = pos.GetY();
	const int z = pos.GetZ();

	Vector3 direction(0.0f, 0.0f, 0.0f);

	if (
		(x >= 0 && x <= getFlowFieldWidth() - 1) &&
		(y >= 0 && y <= getFlowFieldHeight() - 1) &&
		(z >= 0 && z <= getFlowFieldDepth() - 1)
	) {
		if ( false && //bi-linear interpolation
			(x + 1 >= 0 && x + 1 <= getFlowFieldDepth() - 1) &&
			(z + 1 >= 0 && z + 1 <= getFlowFieldDepth() - 1) &&
			m_flowField[x][y][z].isMovable &&
			m_flowField[x][y][z + 1].isMovable &&
			m_flowField[x + 1][y][z].isMovable &&
			m_flowField[x + 1][y][z + 1].isMovable
		) 
		{
			Vector3 bl_direction = m_flowField[x][y][z].direction;
			Vector3 tl_direction = m_flowField[x][y][z + 1].direction;
			Vector3 br_direction = m_flowField[x + 1][y][z].direction;
			Vector3 tr_direction = m_flowField[x + 1][y][z + 1].direction;

			const float bl_area = (position.GetX() - x) * (position.GetZ() - z);
			const float br_area = (ceil(position.GetX()) - x) * (position.GetZ() - z);
			const float tl_area = (position.GetX() - x) * (ceil(position.GetZ()) - z);
			const float tr_area = (ceil(position.GetX()) - x) * (ceil(position.GetZ()) - z);

			direction = ((bl_direction * tr_area) + (br_direction * tl_area) + (tl_direction * br_area) + (tr_direction * bl_area)).Normalize();
		} 
		else {
			direction = m_flowField[x][y][z].direction;
		}
	}

	return direction;
}

const Vector3 FlowField::getDestination()
{
	return m_destination;
}

};
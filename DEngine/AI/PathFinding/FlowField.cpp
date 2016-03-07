#include "FlowField.h"
#include "../../Graphics/MeshComponent.h"
#include "../../Graphics/D3D11Renderer.h"
#include "../../Graphics/Scene/SceneGraph.h"

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

	for (int i = 0; i < getFlowFieldWidth(); i++)
	{
		//for (int j = 0; j < getFlowFieldHeight(); j++)
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < getFlowFieldDepth(); k++)
			{
				if (m_flowField[i][j][k].isMovable)
				{
					Matrix4 rotate, translate;
					rotate.CreateRotationY(-getDirectionInRadian(m_flowField[i][j][k].direction));
					translate.CreateTranslation(Vector3(i + 0.5f, j, k + 0.5f));

					Vector3 arrows[4] = {
						Vector3(-0.3f, 0.0f, 0.0f),
						Vector3(0.3f, 0.0f, 0.0f),
						Vector3(0.15f, 0.0f, 0.15f),
						Vector3(0.15f, 0.0f, -0.15f)
					};

					for (int l = 0; l < 4; l++)
					{
						arrows[l].Transform(rotate);
						arrows[l].Transform(translate);
						vertices[index_ver++].m_pos = arrows[l];
					}

					indices[index_in++] = index_ver + 0;
					indices[index_in++] = index_ver + 1;
					indices[index_in++] = index_ver + 2;
					indices[index_in++] = index_ver + 1;
					indices[index_in++] = index_ver + 3;
					indices[index_in++] = index_ver + 1;
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

	if (
		(x >= 0 && x <= getFlowFieldWidth() - 1) &&
		(y >= 0 && y <= getFlowFieldHeight() - 1) &&
		(z >= 0 && z <= getFlowFieldDepth() - 1) &&
		m_flowField[x][y][z].isMovable
	) {
		return m_flowField[x][y][z].direction;
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

const Vector3 FlowField::getDestination()
{
	return m_destination;
}
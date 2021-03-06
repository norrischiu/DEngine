#include "FlowField.h"
#include "Graphics/MeshComponent.h"
#include "Graphics/D3D11Renderer.h"
#include "Graphics/Scene/SceneGraph.h"
#include "FlowFieldBuilder.h"

namespace DE
{

float getDirectionInRadian(const Vector3& direction)
{
	return atan2(direction.GetZ(), direction.GetX());
}

FlowField::FlowField(const InitInfo initInfo, std::vector<Cell> flowField, std::vector<Vector3> obstacles, const Vector3& offset, const Vector3& destination)
	: m_initInfo(initInfo), m_flowField(flowField), m_obstacles(obstacles), m_offset(offset), m_destination(destination), m_currDir(Vector3(0.0f, 0.0f, 0.0f))
{
}

FlowField::~FlowField()
{
}

void FlowField::Draw()
{
	const int iNumVertices = (m_initInfo.FlowFieldWidth + 1) * 2 + (m_initInfo.FlowFieldDepth + 1) * 2 + 4 * m_initInfo.FlowFieldWidth * m_initInfo.FlowFieldDepth;
	const int iNumIndices = (m_initInfo.FlowFieldWidth + 1) * 2 + (m_initInfo.FlowFieldDepth + 1) * 2 + 6 * m_initInfo.FlowFieldWidth * m_initInfo.FlowFieldDepth;
	Vertex1P* vertices = new Vertex1P[iNumVertices];
	unsigned int* indices = new unsigned int[iNumIndices];

	int index_ver = 0;
	int index_in = 0;
	for (int z = 0; z <= m_initInfo.FlowFieldDepth; z++)
	{
		vertices[index_ver++].m_pos = Vector3(0.0f, 0.0f, z);
		vertices[index_ver++].m_pos = Vector3(m_initInfo.FlowFieldWidth, 0.0f, z);
		indices[index_in] = index_in++;
		indices[index_in] = index_in++;
	}

	for (int x = 0; x <= m_initInfo.FlowFieldWidth; x++)
	{
		vertices[index_ver++].m_pos = Vector3(x, 0.0f, 0.0f);
		vertices[index_ver++].m_pos = Vector3(x, 0.0f, m_initInfo.FlowFieldDepth);
		indices[index_in] = index_in++;
		indices[index_in] = index_in++;
	}

	Vector3 dest = m_destination;
	Matrix4 transform;
	transform.CreateTranslation(-m_offset);
	dest.Transform(transform);

	const int destX = floor(dest.GetX());
	const int destZ = floor(dest.GetZ());

	for (int z = 0; z < m_initInfo.FlowFieldDepth; z++)
	{
		for (int x = 0; x < m_initInfo.FlowFieldWidth; x++)
		{
			const int index = z * m_initInfo.FlowFieldWidth + x;

			Matrix4 translate;
			translate.CreateTranslation(Vector3(x + 0.5f, 0.0f, z + 0.5f));

			if ((destX - x) + (destZ - z) < std::numeric_limits<float>::epsilon()) { //destination
				Vector3 t[4] = {
					Vector3(-0.3f, 0.0f, 0.3f),
					Vector3(0.3f, 0.0f, 0.3f),
					Vector3(0.0f, 0.0f, 0.3f),
					Vector3(0.0f, 0.0f, -0.3f)
				};

				for (int l = 0; l < 4; l++)
				{
					t[l].Transform(translate);
					vertices[index_ver].m_pos = t[l];
					indices[index_in++] = index_ver++;
				}

				continue;
			}

			if (m_flowField[index].isMovable)
			{
				Matrix4 rotate;
				rotate.CreateRotationY(-getDirectionInRadian(m_flowField[index].direction));

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

				indices[index_in++] = index_ver - 4 + 0;
				indices[index_in++] = index_ver - 4 + 1;
				indices[index_in++] = index_ver - 4 + 2;
				indices[index_in++] = index_ver - 4 + 1;
				indices[index_in++] = index_ver - 4 + 3;
				indices[index_in++] = index_ver - 4 + 1;
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

	Handle hMesh(sizeof(MeshComponent));
	new (hMesh) MeshComponent(new MeshData(vertices, iNumVertices, indices, iNumIndices, sizeof(Vertex1P)));
	MeshComponent* meshComponent = (MeshComponent*)hMesh.Raw();

	RenderPass* renderPass = new RenderPass;
	renderPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P.hlsl");
	renderPass->SetPixelShader("../DEngine/Shaders/PS_red.hlsl");
	renderPass->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	meshComponent->m_pMeshData->m_Material.AddPassToTechnique(renderPass);
	renderPass->SetRenderTargets(&((D3D11Renderer*)D3DRenderer::GetInstance())->m_backbuffer->GetRTV(), 1);
	renderPass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
	renderPass->SetRasterizerState(State::CULL_NONE_RS);
	Matrix4 translate;
	translate.CreateTranslation(m_offset);
	meshComponent->m_pTransform->Multiply(translate);

	GameObject* flowField = new GameObject;
	flowField->AddComponent(meshComponent);
	flowField->TransformBy(translate);

	delete[] vertices;
	delete[] indices;


}

int FlowField::distance(const Vector3& position, const Vector3& destination)
{
	Matrix4 transform;
	transform.CreateTranslation(-m_offset);
	Vector3 pos = position;
	Vector3 dest = destination;
	pos.Transform(transform);
	dest.Transform(transform);

	const int pos_x = floor(pos.GetX());
	const int pos_z = floor(pos.GetZ());
	const int dest_x = floor(dest.GetX());
	const int dest_z = floor(dest.GetZ());

	return abs(dest_x - pos_x) + abs(dest_z - pos_z);
}

bool FlowField::isValid(const Vector3& position)
{
	Matrix4 transform;
	transform.CreateTranslation(-m_offset);
	Vector3 pos = position;
	pos.Transform(transform);

	const int x = floor(pos.GetX());
	const int z = floor(pos.GetZ());

	return (
		(x >= 0 && x <= m_initInfo.FlowFieldWidth - 1) &&
		(z >= 0 && z <= m_initInfo.FlowFieldDepth - 1)
	);
}

bool FlowField::isPositionMovable(const Vector3& position)
{
	Matrix4 transform;
	transform.CreateTranslation(-m_offset);
	Vector3 pos = position;
	pos.Transform(transform);

	const int x = floor(pos.GetX());
	const int z = floor(pos.GetZ());

	if (isValid(position)) {
		const int index = z * m_initInfo.FlowFieldWidth + x;
		return m_flowField[index].isMovable;
	}

	return false;
}

const Vector3 FlowField::transfromAndFloor(const Vector3& position)
{
	Matrix4 transform;
	transform.CreateTranslation(-m_offset);
	Vector3 pos = position;
	pos.Transform(transform);

	pos.SetX(floor(pos.GetX()));
	pos.SetY(floor(pos.GetY()));
	pos.SetZ(floor(pos.GetZ()));

	return pos;
}

const Vector3 FlowField::getDirection(const Vector3& position)
{
	Matrix4 transform;
	transform.CreateTranslation(-m_offset);
	Vector3 pos = position;

	if (isPositionMovable(pos))
	{
		const int intMax = (std::numeric_limits<int>::max)();
		const int f00 = isPositionMovable(pos) ? distance(pos, m_destination) : intMax;
		const int f01 = isPositionMovable(pos + Vector3(0.0f, 0.0f, 1.0f)) ? distance(pos + Vector3(0.0f, 0.0f, 1.0f), m_destination) : intMax;
		const int f10 = isPositionMovable(pos + Vector3(1.0f, 0.0f, 0.0f)) ? distance(pos + Vector3(1.0f, 0.0f, 0.0f), m_destination) : intMax;
		const int f11 = isPositionMovable(pos + Vector3(1.0f, 0.0f, 1.0f)) ? distance(pos + Vector3(1.0f, 0.0f, 1.0f), m_destination) : intMax;

		int minVal = min(min(f00, f01), min(f10, f11));
		std::vector<Vector3> minCoord;

		if (f00 == minVal) minCoord.push_back(pos);
		if (f01 == minVal) minCoord.push_back(pos + Vector3(0.0f, 0.0f, 1.0f));
		if (f10 == minVal) minCoord.push_back(pos + Vector3(1.0f, 0.0f, 0.0f));
		if (f11 == minVal) minCoord.push_back(pos + Vector3(1.0f, 0.0f, 1.0f));

		Vector3 currDirection = m_currDir;
		Vector3 desiredDirection;
		minVal = intMax;

		for (int i = 0; i < minCoord.size(); i++)
		{
			Vector3 m = minCoord[i];
			Vector3 p = pos;
			m = transfromAndFloor(m);
			p = transfromAndFloor(p);

			const int index = p.GetZ() * m_initInfo.FlowFieldWidth + p.GetX();

			const Vector3 directionTo = (m - p).iszero() ? m_flowField[index].direction : (m - p).Normalize();
			const int length = (directionTo - currDirection).Length();

			if (length < minVal)
			{
				minVal = length;
				desiredDirection = directionTo;
			}
		}

		m_currDir = desiredDirection;

		return m_currDir;
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

std::vector<Vector3>& FlowField::getObstacles()
{
	return m_obstacles;
}


const Vector3 FlowField::getOffset()
{
	return m_offset;
}

const Vector3 FlowField::getDestination()
{
	return m_destination;
}

};
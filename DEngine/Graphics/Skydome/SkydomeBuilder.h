#pragma once
#include "../../GameObject/GameObject.h"
#include "../../Graphics/VertexFormat.h"

namespace DE
{

class SkydomeBuilder
{
public:
	SkydomeBuilder();
	~SkydomeBuilder();

public:
	static SkydomeBuilder* getInstance();
	GameObject* CreateGameObject(const char* filename = "skydome.txt", const Vector3& apexColor = Vector3(0.0f, 0.05f, 0.6f, 1.0f), const Vector3& centerColor = Vector3(0.0f, 0.5f, 0.8f, 1.0f));

private:
	bool LoadSkyDomeModel(const char* filename);

private:
	static SkydomeBuilder* m_instance;
	Vertex1P1N1D* m_vertices;
	int m_iNumVertices;
};

};
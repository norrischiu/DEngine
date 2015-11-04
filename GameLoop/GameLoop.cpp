#include "GameLoop.h"

GameLoop::GameLoop()
{
	CreateGameContent();
}


GameLoop::~GameLoop()
{

}

void GameLoop::CreateGameContent()
{
	//Two vertices
	MeshInstance* m1 = m_debug.draw_line(Vector3(3.0f, 3.0f, 1.0f), Vector3(3.0f, 2.0f, 1.0f));
	m1->GetMeshData()->Transform(1.0f, Vector3(0.0f, 0.0f, 0.0f), Vector3(3.0f, 3.0f, 0.0f));

	MeshInstance* m2 = m_debug.draw_line(Vector3(-3.0f, 3.0f, 1.0f), Vector3(-3.0f, 2.0f, 1.0f));
	m2->GetMeshData()->Transform(1.0f, Vector3(0.0f, 0.0f, 0.0f), Vector3(-3.0f, 3.0f, 0.0f));

	//Vector3(width, height depth), num_slice, num_stack)
	MeshInstance* m3 = m_debug.draw_prism(Vector3(1.0f, 1.5f, 2.0f), Primitives::CYLINDER);
	m3->GetMeshData()->Transform(1.0f, Vector3(0.0f, 0.0f, 0.0f), Vector3(3.0f, 0.0f, 0.0f));

	MeshInstance* m4 = m_debug.draw_pyramid(Vector3(1.0f, 1.5f, 2.0f), Primitives::CONE, 30);
	m4->GetMeshData()->Transform(1.0f, Vector3(0.0f, 0.0f, 0.0f), Vector3(-3.0f, 0.0f, 0.0f));

	MeshInstance* m5 = m_debug.draw_pyramid(Vector3(1.0f, 1.5f, 2.0f), Primitives::RECTANGULAR_PYRAMID, 30);
	m5->GetMeshData()->Transform(1.0f, Vector3(0.0f, 0.0f, 0.0f), Vector3(3.0f, -3.0f, 0.0f));

	MeshInstance* m6 = m_debug.draw_ellipsoid(Vector3(1.0f, 1.5f, 2.0f), Primitives::ELLIPSOID, 30);
	m6->GetMeshData()->Transform(1.0f, Vector3(0.0f, 0.0f, 0.0f), Vector3(-3.0f, -3.0f, 0.0f));

	MeshInstance* m7 = m_font.write(ALL_SUPPORT_CHARACTER);	 //All supported characters
	m7->GetMeshData()->Transform(0.5f, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

	// Import .obj object  
	// (param1: model name defined at String.h, param2: the scaling of the object)
	if (m_objectLoader.LoadWaveFrontObject(CCD_SMARTCAR, 0.5f)) {
		std::vector<MeshInstance*> m8 = m_objectLoader.Draw();
		 // for (MeshInstance* meshInstance : m8) { } 
	}

	//All objects translate to left by 5 unit;
	for (MeshInstance* meshInstance : D3D11Renderer::GetInstance()->GetMeshInstanceList()) {
		meshInstance->GetMeshData()->Transform(
			1.0f,											//scaling
			Vector3(0.0f, 0.0f, 0.0f),						//rotation
			Vector3(5.0f, 0.0f, 0.0f)						//translation
		);
	}
}

void GameLoop::Update(const float elaspedTime)
{
	if (elaspedTime < 100.0f) {		
		for (MeshInstance* meshInstance : D3D11Renderer::GetInstance()->GetMeshInstanceList()) {
			meshInstance->GetMeshData()->Transform(
				1.0f,											//scaling
				Vector3(0.0f, 0.0f, 0.0f),						//rotation
				Vector3(-5.0f * elaspedTime, 0.0f, 0.0f)		//translation
			);
		}
	}
}
#ifndef DEBUG_RENDERER_H_
#define DEBUG_RENDERER_H_

#include "debug.h"
#include "Math\Frustum.h"
#include "Physics\cdAabb.h"
#include "Graphics\HUD\HUDElement\TextBox\TextEngine.h"
#include "D3D11Renderer.h"
#include "Graphics\Scene\SceneGraph.h"

class DEBUG_RENDERER
{

public:

	void DRAW_FRUSTUM(Frustum frustum)
	{
		//Debug drawer;
		//drawer.draw_line();
	}

	void DRAW_AABB(AABB box)
	{
		Debug drawer;
		Vector3 corners[8];
		Vector3 m_Min = box.getMin(), m_Max = box.getMax();
		corners[0] = m_Min;
		corners[1] = Vector3(m_Min.GetX(), m_Max.GetY(), m_Min.GetZ());
		corners[2] = Vector3(m_Max.GetX(), m_Max.GetY(), m_Min.GetZ());
		corners[3] = Vector3(m_Max.GetX(), m_Min.GetY(), m_Min.GetZ());
		corners[4] = Vector3(m_Min.GetX(), m_Min.GetY(), m_Max.GetZ());
		corners[5] = Vector3(m_Min.GetX(), m_Max.GetY(), m_Max.GetZ());
		corners[6] = m_Max;
		corners[7] = Vector3(m_Max.GetX(), m_Min.GetY(), m_Max.GetZ());
		drawer.draw_line(corners[0], corners[1]);
		drawer.draw_line(corners[1], corners[2]);
		drawer.draw_line(corners[2], corners[3]);
		drawer.draw_line(corners[3], corners[0]);
		drawer.draw_line(corners[4], corners[5]);
		drawer.draw_line(corners[5], corners[6]);
		drawer.draw_line(corners[6], corners[7]);
		drawer.draw_line(corners[7], corners[4]);
		drawer.draw_line(corners[0], corners[4]);
		drawer.draw_line(corners[1], corners[5]);
		drawer.draw_line(corners[2], corners[6]);
		drawer.draw_line(corners[3], corners[7]);
	}
	
	void DRAW_2D_TEXT(const char* text, unsigned int screenX, unsigned int screenY)
	{
		TextEngine txtHelper;
		MeshData* meshData = txtHelper.CreateTextMeshData(text);
		//m_vDbgMeshs.push_back(meshData);
		RenderPass* pass = meshData->m_Material.GetRenderTechnique()->m_vRenderPasses[0];
		pass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
		pass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
		pass->SetRasterizerState(State::CULL_NONE_RS);
		MeshComponent* meshComp = new MeshComponent(meshData);
		Matrix4 scaleX, scaleY;
		scaleX.CreateScaleX(-1024.0f / 2.0f);
		scaleY.CreateScaleY(768.0f / 2.0f);
		*meshComp->m_pTransform = m_m2DProjection * scaleX * scaleY;
		SceneGraph::GetInstance()->ADD_DEBUG_DRAWING(meshComp);
	}

	void DRAW_LINE_LIST(float* points)
	{
	}

	static DEBUG_RENDERER* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new DEBUG_RENDERER;
		}
		return m_pInstance;
	}

	void PreRender()
	{
		// insert debug mesh to scene graph,
		// or directly add it in above functions
	}

	void Render()
	{

	}

	void PostRender()
	{
		m_vDbgMeshs.clear();
	}

private:
	
	DEBUG_RENDERER();

	// Singleton instance
	static DEBUG_RENDERER*						m_pInstance;

	// Array of debug mesh
	std::vector<MeshData*>						m_vDbgMeshs;

	// Default orthgraphic projection
	Matrix4										m_m2DProjection;
};

#endif // !DEBUG_RENDERER_H_


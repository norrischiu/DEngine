#ifndef DEBUG_RENDERER_H_
#define DEBUG_RENDERER_H_

#include "debug.h"
#include "Math\Frustum.h"
#include "Physics\cdAabb.h"
#include "Helpers\TextHelper.h"
#include "Graphics\Scene\SceneGraph.h"
#include "Utilities\MyArray.h"
#include "Graphics\D3D12Renderer.h"

namespace DE
{

class DEBUG_RENDERER
{

public:

	enum DEBUG_RENDER_FLAG
	{
		TEXT2D, MESH2D_TEXTURE, MESH3D_WIREFRAME
	};

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
		m_vDbgMeshs.Add(drawer.draw_line(corners[0], corners[1]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[1], corners[2]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[2], corners[3]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[3], corners[0]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[4], corners[5]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[5], corners[6]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[6], corners[7]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[7], corners[4]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[0], corners[4]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[1], corners[5]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[2], corners[6]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
		m_vDbgMeshs.Add(drawer.draw_line(corners[3], corners[7]));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
	}

	void DRAW_2D_TEXT(const char* text, unsigned int screenX, unsigned int screenY)
	{
		TextHelper txtHelper;
		MeshData* meshData = txtHelper.CreateTextMeshData(text);
		m_vDbgMeshs.Add(meshData);
	}

	void DRAW_2D_MESH(MeshData* meshdata)
	{
	}

	void DRAW_LINE_LIST(Vector3* points)
	{
	}

	void DRAW_RAY_SEGMENT(Vector3 start, Vector3 end)
	{
		Debug drawer;
		m_vDbgMeshs.Add(drawer.draw_line(start, end));
		m_vRenderFlags.Add(MESH3D_WIREFRAME);
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

	void Render(Renderer* renderer)
	{
		m_VSCBuffer.BindToRenderer();
		VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_VSCBuffer.m_Memory._data;

		const unsigned int size = m_vDbgMeshs.Size();
		for (int i = 0; i < size; ++i)
		{
			if (m_vRenderFlags[i] == MESH3D_WIREFRAME)
			{
				m_3DRenderPass.SetVertexShader("../DEngine/Shaders/VS_vertex1P.hlsl");
				m_3DRenderPass.SetPixelShader("../DEngine/Shaders/PS_red.hlsl");
				m_3DRenderPass.SetRasterizerState(State::WIREFRAME_RS);

				ptr->WVPTransform = renderer->GetCamera()->GetPVMatrix();
				m_VSCBuffer.Update();
			}
			else if (m_vRenderFlags[i] == MESH2D_TEXTURE)
			{
				m_3DRenderPass.SetVertexShader("../DEngine/Shaders/VS_vertex1P1UV.hlsl");
				m_3DRenderPass.SetPixelShader("../DEngine/Shaders/PS_texture.hlsl");
				m_3DRenderPass.SetRasterizerState(State::WIREFRAME_RS);

				ptr->WVPTransform = m_m2DProjection;
				m_VSCBuffer.Update();
			}
			m_3DRenderPass.BindToRenderer(renderer);
			m_vDbgMeshs[i]->Render(renderer);
		}
		ptr->WVPTransform = m_m2DProjection;
		m_VSCBuffer.Update();
		hpMeterBorder->Render(renderer);

		// Demo purpose
		Matrix4 trans, scale;
		scale.CreateScaleX(boosHPWidth / 800.0f);
		trans.CreateTranslation(Vector3((boosHPWidth - 800.0f) / 2.0, 0.0f, 0.0f));
		ptr->WVPTransform = m_m2DProjection * trans * scale;
		m_VSCBuffer.Update();
		hpMeter->Render(renderer);

		Matrix4 trans2;
		trans2.CreateTranslation(Vector3(830.0f, 0.0f, 0.0f));
		scale.CreateScaleX(PlayerStaminaWidth / 100.0f);
		trans.CreateTranslation(Vector3(-830.0f + (PlayerStaminaWidth - 100.0f) / 2.0, 0.0f, 0.0f));
		ptr->WVPTransform = m_m2DProjection * trans * scale * trans2;
		m_VSCBuffer.Update();
		staminaMeter->Render(renderer);

		trans2.CreateTranslation(Vector3(805.0f, 0.0f, 0.0f));
		scale.CreateScaleX(PlayerHpWidth / 150.0f);
		trans.CreateTranslation(Vector3(-805.0f + (PlayerHpWidth - 150.0f) / 2.0, 0.0f, 0.0f));
		ptr->WVPTransform = m_m2DProjection * trans * scale * trans2;
		m_VSCBuffer.Update();
		playerHpMeter->Render(renderer);

		PostRender();
	}

	void PostRender()
	{
		const unsigned int size = m_vDbgMeshs.Size();
		for (int i = 0; i < size; ++i)
		{
			//m_vDbgMeshs[i]->m_Material.Destruct();
			delete m_vDbgMeshs[i];
		}
		m_vDbgMeshs.Clear();
		m_vRenderFlags.Clear();
	}

private:

	DEBUG_RENDERER();

	// Singleton instance
	static DEBUG_RENDERER*						m_pInstance;

	// Array of debug mesh
	MyArray<MeshData*>							m_vDbgMeshs;

	MyArray<int>								m_vRenderFlags;

	// Default orthgraphic projection
	Matrix4										m_m2DProjection;

	// Rende pass for 3D debug mesh
	RenderPass									m_3DRenderPass;

	// Constant buffer
	VSPerObjectCBuffer							m_VSCBuffer;

	// Demo purpose
	MeshData* hpMeter;
	MeshData* hpMeterBorder;
	MeshData* staminaMeter;
	MeshData* playerHpMeter;
public:
	float boosHPWidth;
	float PlayerStaminaWidth;
	float PlayerHpWidth;
};

};
#endif // !DEBUG_RENDERER_H_


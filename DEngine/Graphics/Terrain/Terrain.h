#pragma once

#include <vector>
#include "Math\simdmath.h"
#include "Graphics\D3D11Renderer.h"
#include "Graphics\Render\HSDSPerFrameCBuffer.h"

namespace DE
{

	struct float2
	{
		float x;
		float y;

		float2() {}
		float2(const float x, const float y) : x(x), y(y) { }
	};

	class Terrain
	{
	public:
		struct InitInfo
		{
			int HeightmapWidth;
			int HeightmapHeight;
			float HeightScale;
			float CellSpacing;
			int CellsPerPatch;

			InitInfo(const int HeightmapWidth, const int HeightmapHeight, const float HeightScale, const float CellSpacing, const int CellsPerPatch)
				: HeightScale(HeightScale), HeightmapWidth(HeightmapWidth), HeightmapHeight(HeightmapHeight), CellSpacing(CellSpacing), CellsPerPatch(CellsPerPatch)
			{

			}

			InitInfo() {}
		};

		static Terrain*	GetInstance()
		{
			if (!m_pInstance)
				m_pInstance = new Terrain;
			return m_pInstance;
		}

	public:
		Terrain(){}
		Terrain(const InitInfo initInfo, std::vector<float> heightMap);
		~Terrain();

	public:
		// get the total terrain width
		float GetWidth()const;

		// get the total terrain depth
		float GetDepth()const;

		// get the height of the cell
		float GetHeight(float x, float z) const;

	public:
		GameObject* CreateGameObject(const char* diffuseTxt_filename, const char* normalTxt_filename);

	private:
		std::vector<float2> CalcAllPatchBoundsY();
		float2 CalcPatchBoundsY(const int patch_id);
		std::vector<float2> calculateTextureCoordiate();
		std::vector<Vector3> calculateNormal();
		const Vector3 calculateTangent(const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3, const float2 vertex1_uv, const float2 vertex2_uv, const float2 vertex3_uv);

	private:
		std::vector<float>	m_HeightMap;
		InitInfo m_initInfo;

	private:
		HSDSPerFrameCBuffer*			m_pHSDSCBuffer;
		int cellPerPatch = 64;
		int m_numPatchRows;
		int m_numPatchCols;
		static Terrain* m_pInstance;
	};
};

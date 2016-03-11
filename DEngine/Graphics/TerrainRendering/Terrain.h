
#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <vector>
#include "Math\simdmath.h"

namespace DE
{
struct float2
{
	float x;
	float y;
};

class Terrain
{
public:

	struct InitInfo
	{
		std::wstring HeightMapFilename;
		std::wstring LayerMapFilename0;
		std::wstring LayerMapFilename1;
		std::wstring LayerMapFilename2;
		std::wstring LayerMapFilename3;
		std::wstring LayerMapFilename4;
		std::wstring BlendMapFilename;
		float HeightScale;
		unsigned int HeightmapWidth;
		unsigned int HeightmapHeight;
		float CellSpacing;
	};

	Terrain();

	~Terrain();

	float GetWidth()const;

	float GetDepth()const;

	float GetHeight(float x, float z)const;

	void LoadHeightMap();

	

private:
	InitInfo m_Info;

	std::vector<float2> m_PatchBoundsY;
	std::vector<float> m_HeightMap;

};
};

#endif // !TERRAIN_H_

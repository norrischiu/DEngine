
#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <vector>
#include "Math\simdmath.h"
#include "Graphics\D3D11Renderer.h"


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
		std::string HeightMapFilename;
		std::string LayerMapFilename0;
		float HeightScale;
		unsigned int HeightmapWidth;
		unsigned int HeightmapHeight;
		float CellSpacing;
	};

	Terrain();

	~Terrain();

	// input the initial information of the height map.
	void Init(const char* heightMapFile, const char* layerMapFile, float heightScale, unsigned int heightMapWidth, unsigned int heightMapHeight, float cellSpacing);

	// get the total terrain width
	float GetWidth()const;

	// get the total terrain depth
	float GetDepth()const;

	// get the height of the cell
	float GetHeight(float x, float z)const;

	void Draw();

	

private:

	// total number of patch vertices
	unsigned int mNumPatchVertices;

	// total number of quad patch primitives
	unsigned int mNumPatchQuadFaces;

	// number of the patch in a row of the vertex grid dimension
	unsigned int mNumPatchVertRows;

	// number of the patch in a column of the vertex grid dimension
	unsigned int mNumPatchVertCols;

	void LoadHeightMap();

	InitInfo m_Info;

	std::vector<float2> m_PatchBoundsY;
	
	std::vector<float>	m_HeightMap;

	// mesh for the quad patch VB
	MeshData*			m_QuadPatchMesh;

};
};

#endif // !TERRAIN_H_

#pragma once
#include "Terrain.h"

namespace DE
{

class TerrainBuilder
{
public:
	TerrainBuilder();
	~TerrainBuilder();

	static TerrainBuilder* getInstance();
	Terrain* generateTerrain(const char* heightMapFile, const unsigned int heightMapWidth, const unsigned int heightMapHeight, const float heightScale, const float cellSpacing, const int cellsPerPatch = 64);

private:
	std::vector<float> LoadHeightMap(const char* heightMapFile, const float heightScale, const int heightmapWidth, const int heightmapHeight);

	static TerrainBuilder* m_instance;
};

};


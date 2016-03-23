#include "TerrainBuilder.h"
#include <fstream>

namespace DE
{

TerrainBuilder* TerrainBuilder::m_instance = nullptr;

TerrainBuilder::TerrainBuilder()
{
}

TerrainBuilder::~TerrainBuilder()
{
}



TerrainBuilder* TerrainBuilder::getInstance()
{
	if (!m_instance) {
		m_instance = new TerrainBuilder;
	}

	return m_instance;
}

std::vector<float> TerrainBuilder::LoadHeightMap(const char* heightMapFile, const float heightScale, const int heightmapWidth, const int heightmapHeight)
{
	
	const int imageSize = heightmapWidth * heightmapHeight;
	unsigned char* bitmapImage = new unsigned char[imageSize];
	std::vector<float> m_HeightMap(imageSize);

	FILE* filePtr;
	fopen_s(&filePtr, heightMapFile, "rb");

	fread(bitmapImage, 1, imageSize, filePtr);

	// Copy the array data into a float array and scale it.
	for (int j = 0; j < heightmapHeight; j++)
	{
		for (int i = 0; i < heightmapWidth; i++)
		{
			const int index = (heightmapWidth * j) + i;

			m_HeightMap[index] = ((float) bitmapImage[index] / 255.0f) * heightScale;
		}
	}

	delete[] bitmapImage;

	return m_HeightMap;
}

Terrain* TerrainBuilder::generateTerrain(const char* heightMapFile, const char* layerMapFile, const unsigned int heightMapWidth, const unsigned int heightMapHeight, const float heightScale, const float cellSpacing, const int cellsPerPatch)
{
	std::string heightMapPath(heightMapFile);
	std::string layerMapPath(layerMapFile);
	heightMapPath = "../Assets/" + heightMapPath;
	//layerMapPath = "../Assets/" + layerMapPath;

	Terrain::InitInfo initInfo(heightMapWidth, heightMapHeight, heightScale, cellSpacing, cellsPerPatch);
	std::vector<float> m_heightMap = LoadHeightMap(heightMapPath.c_str(), heightScale, heightMapWidth, heightMapHeight);

	Terrain* terrain = new Terrain(initInfo, m_heightMap);

	return terrain;
}

};
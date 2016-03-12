#include "Terrain.h"
#include <fstream>

namespace DE
{

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

void Terrain::Init(const char * heightMapFile, const char * layerMapFile, float heightScale, unsigned int heightMapWidth, unsigned int heightMapHeight, float cellSpacing)
{
	// pass data to m_Info
	std::string heightMapName(heightMapFile);
	std::string layerMapName(layerMapFile);
	heightMapName = "../Assets/" + heightMapName;
	layerMapName = "../Assets/" + layerMapName;

	m_Info.HeightMapFilename = heightMapName;
	m_Info.LayerMapFilename0 = layerMapName;
	m_Info.HeightmapWidth = heightMapWidth;
	m_Info.HeightmapHeight = heightMapHeight;
	m_Info.CellSpacing = cellSpacing;



}

float Terrain::GetWidth() const
{
	return (m_Info.HeightmapWidth - 1)*m_Info.CellSpacing;
}

float Terrain::GetDepth() const
{
	// Total terrain depth.
	return (m_Info.HeightmapHeight - 1)*m_Info.CellSpacing;
}

float Terrain::GetHeight(float x, float z) const
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*GetWidth()) / m_Info.CellSpacing;
	float d = (z - 0.5f*GetDepth()) / -m_Info.CellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	float A = m_HeightMap[row*m_Info.HeightmapWidth + col];
	float B = m_HeightMap[row*m_Info.HeightmapWidth + col + 1];
	float C = m_HeightMap[(row + 1)*m_Info.HeightmapWidth + col];
	float D = m_HeightMap[(row + 1)*m_Info.HeightmapWidth + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s)*uy + (1.0f - t)*vy;
	}
}


void Terrain::LoadHeightMap()
{
	std::vector<unsigned char> in(m_Info.HeightmapWidth * m_Info.HeightmapHeight);

	// Open the file.
	std::ifstream inFile;
	inFile.open(m_Info.HeightMapFilename.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array and scale it.
	m_HeightMap.resize(m_Info.HeightmapHeight * m_Info.HeightmapWidth, 0);
	for (unsigned int i = 0; i < m_Info.HeightmapHeight * m_Info.HeightmapWidth; ++i)
	{
		m_HeightMap[i] = (in[i] / 255.0f)*m_Info.HeightScale;
	}
}
};
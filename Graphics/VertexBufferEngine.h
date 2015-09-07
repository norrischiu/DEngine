// VertexBufferEngine.h: 

#include "D3D11Renderer.h"

class VertexBufferEngine
{
public:
	VertexBufferEngine() {};

	void CreateBuffer(const char* filename, int vertexFormat);

	void FillVertexData_POSITION(unsigned int vertsNum, void* &pVertexData);

	void FillVertexData_POSITION_TEXTURE(unsigned int vertsNum, void* &pVertexData);

	FILE* pFile;
};
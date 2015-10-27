// VertexBufferEngine.h: 

#include "D3D11Renderer.h"

class VertexBufferEngine
{
public:
	VertexBufferEngine() {};

	void* CreateBuffer(const char* filename, int vertexFormat, unsigned int& stride);

	void FillVertexData_POSITION(const char* filename, unsigned int vertsNum, void* &pVertexData);

	void FillVertexData_POSITION_TEXTURE(const char* filename, unsigned int vertsNum, void* &pVertexData);

	void FillVertexData_POSITION_NORMAL_TEXTURE(const char* filename, unsigned int vertsNum, void* &pVertexData);

	FILE* pFile;
};
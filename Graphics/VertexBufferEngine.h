#include "D3D11Renderer.h"
#include "VertexFormat.h"

class VertexBufferEngine
{
public:
	VertexBufferEngine() {};

	ID3D11Buffer* CreateBufferFromRawData(void* pVertexData, const int iNumVerts, const unsigned int iDataSize);
	void* CreateBuffer(const char* filename, int vertexFormat, unsigned int& stride);

	void FillVertexData_POSITION(const char* filename, unsigned int vertsNum, void* &pVertexData);

	void FillVertexData_POSITION_TEXTURE(const char* filename, unsigned int vertsNum, void* &pVertexData);

	void FillVertexData_POSITION_NORMAL_TEXTURE(const char* filename, unsigned int vertsNum, void* &pVertexData);
	static VertexBufferEngine* GetInstance();
	static void DestructandCleanUp();

private:
	static VertexBufferEngine*		m_pInstance;

	FILE* pFile;
};
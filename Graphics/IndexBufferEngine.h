// IndexBufferEngine.h:

#include "D3D11Renderer.h"

class IndexBufferEngine
{
public:
	IndexBufferEngine() {};

	void* CreateBuffer(const char* filename, unsigned int& indicsNum);

	void FillIndexData(unsigned int indicsNum, void* &pIndexData);

	FILE* pFile;
};
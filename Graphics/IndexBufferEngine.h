// IndexBufferEngine.h:

#include "D3D11Renderer.h"

class IndexBufferEngine
{
public:
	IndexBufferEngine() {};

	void CreateBuffer(const char* filename);

	void FillIndexData(unsigned int indicsNum, void* &pIndexData);

	FILE* pFile;

	static unsigned int g_iCurrIndex;
};
// IndexBufferEngine.h:

#include "D3D11Renderer.h"

class IndexBufferEngine
{
public:

	ID3D11Buffer* CreateBufferFromRawData(const unsigned int* pIndexData, const int m_iNumIndics);
	void* CreateBuffer(const char* filename, unsigned int& indicsNum);

	void FillIndexData(unsigned int indicsNum, void* &pIndexData);

	static IndexBufferEngine* GetInstance();
	static void DestructandCleanUp();

	static unsigned int GetCurrentIndex();

private:
	static IndexBufferEngine*		m_pInstance;
	static unsigned int g_iCurrIndex;
	FILE* pFile;
};
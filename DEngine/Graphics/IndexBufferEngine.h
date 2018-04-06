// IndexBufferEngine.h: helper class to create d3d index buffer from uint array and file

//Engine include
#include "D3D12Renderer.h"
#include "Memory\Handle.h"

namespace DE
{
/*
*	CLASS: IndexBufferEngine
*	IndexBufferEngine is the helper to create D3D index buffer
*	from a file or a raw index array
*/
class IndexBufferEngine
{

public:

	/********************************************************************************
	*	--- Function:
	*	CreateBufferFromRawData(const unsigned int*, const int)
	*	This function will create a D3D index buffer using the render device
	*	by the given data
	*
	*	--- Parameters:
	*	@ pIndexData: the indices array
	*	@ m_iNumIndics: the number of index in the above array
	*
	*	--- Return:
	*	@ void*: the index buffer created using the given data
	********************************************************************************/
	void* CreateBufferFromRawData(const unsigned int* pIndexData, const int m_iNumIndics);

	/********************************************************************************
	*	--- Function:
	*	CreateBuffer(const char*, unsigned int&)
	*	This function will create a D3D index buffer using the render device
	*	by the given file
	*
	*	--- Parameters:
	*	@ filename: the prefix file name of the mesh definition file
	*	(without "_index.bufa")
	*	@ vertexFormat: the vertex format as defined in eVertexFormat
	*	@ indicsNum: the number of indices, return as output
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void* CreateBuffer(const char* filename, unsigned int& indicsNum);

	/********************************************************************************
	*	--- Function:
	*	FillIndexData(unsigned int, Handle)
	*	This function will fill the indices array
	*
	*	--- Parameters:
	*	@ indicsNum: the number of indices
	*	@ hIndexData: the handle referring to the indics array
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void FillIndexData(unsigned int indicsNum, Handle hIndexData);

	/********************************************************************************
	*	--- Static Function:
	*	GetInstance()
	*	This function will return the singleton instance of IndexBufferEngine
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ IndexBufferEngine*: the singleton instance
	********************************************************************************/
	static IndexBufferEngine* GetInstance();

	/********************************************************************************
	*	--- Static Function:
	*	DestructandCleanUp()
	*	This function will delete the singleton instance
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	static void DestructandCleanUp();

private:

	static IndexBufferEngine*			m_pInstance;	// singleton instance
	static unsigned int					g_iCurrIndex;	// the global index, can be used to group indices
	FILE*								pFile;		// pointer to file used by C style file I/O
};

};
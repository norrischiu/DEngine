// Engine include
#include "D3D12Renderer.h"
#include "VertexFormat.h"
#include "Memory\Handle.h"

namespace DE
{

/*
*	CLASS: VertexBufferEngine
*	VertexBufferEngine is the helper to create D3D vertex buffer
*	from a file or a group of raw data. To allow more vertex format,
*	define the way to read from file here
*/
class VertexBufferEngine
{
public:

	/********************************************************************************
	*	--- Constructor:
	*	VertexBufferEngine()
	*	This constructor will construct the engine with starting value for min and max
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	VertexBufferEngine()
		:m_vMaxXYZ(-FLT_MAX, -FLT_MAX, -FLT_MAX)
		, m_vMinXYZ(FLT_MAX, FLT_MAX, FLT_MAX)
	{
	};

	/********************************************************************************
	*	--- Function:
	*	CreateBufferFromRawData(void*, const int, const unsigned int, bool)
	*	This function will create a D3D vertex buffer using the render device 
	*	by the given data
	*
	*	--- Parameters:
	*	@ pVertexData: the vertices array in any format, refer to vertexformat.h for
	*	example
	*	@ iNumVerts: the number of vertex in the above array
	*	@ iDataSize: the size of a vertex in the given array
	*	@ streamOut: True if this vertex buffer will be used as stream out target;
	*	False of this vertex buffer will NOT be used as stream out target
	*
	*	--- Return:
	*	@ void*: GPUVirtualAddress
	********************************************************************************/
	void* CreateBufferFromRawData(void* pVertexData, const int iNumVerts, const unsigned int iDataSize, bool streamOut = false);
	
	/********************************************************************************
	*	--- Function:
	*	CreateBuffer(const char* , int, unsigned int&)
	*	This function will create a D3D vertex buffer using the render device
	*	by the given file
	*
	*	--- Parameters:
	*	@ filename: the prefix file name of the mesh definition file 
	*	(without "_vertex.bufa")
	*	@ vertexFormat: the vertex format as defined in eVertexFormat
	*	@ stride: the size of a vertex read from the file, return as output
	*	@ bufferSize: the size of the vertex buffer in byte
	*
	*	--- Return:
	*	@ void*: result as GPUVirtualAddress
	********************************************************************************/
	void* CreateBuffer(const char* filename, int vertexFormat, unsigned int& stride, unsigned int& bufferSize);

	/********************************************************************************
	*	--- Function:
	*	FillVertexData_POSITION(const char*, unsigned int, Handle)
	*	This function will fill the vertex array with position data
	*
	*	--- Parameters:
	*	@ filename: the prefix file name of the mesh definition file
	*	(without "_vertex.bufa")
	*	@ vertsNum: the number of vertex
	*	@ hVertexData: the handle referring to the vertices array
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void FillVertexData_POSITION(const char* filename, unsigned int vertsNum, Handle hVertexData);

	/********************************************************************************
	*	--- Function:
	*	FillVertexData_POSITION_TEXTURE(const char*, unsigned int, Handle)
	*	This function will fill the vertex array with position and texture coordinate
	*	data
	*
	*	--- Parameters:
	*	@ filename: the prefix file name of the mesh definition file
	*	(without "_vertex.bufa")
	*	@ vertsNum: the number of vertex
	*	@ hVertexData: the handle referring to the vertices array
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void FillVertexData_POSITION_TEXTURE(const char* filename, unsigned int vertsNum, Handle hVertexData);

	/********************************************************************************
	*	--- Function:
	*	FillVertexData_POSITION_NORMAL_TEXTURE(const char*, unsigned int, Handle)
	*	This function will fill the vertex array with position, texture coordinate
	*	and normal data
	*
	*	--- Parameters:
	*	@ filename: the prefix file name of the mesh definition file
	*	(without "_vertex.bufa")
	*	@ vertsNum: the number of vertex
	*	@ hVertexData: the handle referring to the vertices array
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void FillVertexData_POSITION_NORMAL_TEXTURE(const char* filename, unsigned int vertsNum, Handle hVertexData);

	/********************************************************************************
	*	--- Function:
	*	FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE(const char*, unsigned int, Handle)
	*	This function will fill the vertex array with position, texture coordinate
	*	normal and tangent data
	*
	*	--- Parameters:
	*	@ filename: the prefix file name of the mesh definition file
	*	(without "_vertex.bufa")
	*	@ vertsNum: the number of vertex
	*	@ hVertexData: the handle referring to the vertices array
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE(const char* filename, unsigned int vertsNum, Handle hVertexData);

	/********************************************************************************
	*	--- Function:
	*	FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE_FOUR_JOINTS(const char*, unsigned int, Handle)
	*	This function will fill the vertex array with position, texture coordinate
	*	normal, tangent and skin weight data
	*
	*	--- Parameters:
	*	@ filename: the prefix file name of the mesh definition file
	*	(without "_vertex.bufa")
	*	@ vertsNum: the number of vertex
	*	@ hVertexData: the handle referring to the vertices array
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE_FOUR_JOINTS(const char* filename, unsigned int vertsNum, Handle hVertexData);

	/********************************************************************************
	*	--- Function:
	*	GetMaxXYZ()
	*	This function will return the vector containing the max position of the last
	*	processed vertex file
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ Vector3: the max position in x, y, z
	********************************************************************************/
	Vector3 GetMaxXYZ()
	{
		Vector3 result = m_vMaxXYZ;
		m_vMaxXYZ = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		return result;
	}

	/********************************************************************************
	*	--- Function:
	*	GetMinXYZ()
	*	This function will return the vector containing the min position of the last
	*	processed vertex file
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ Vector3: the min position in x, y, z
	********************************************************************************/
	Vector3 GetMinXYZ()
	{
		Vector3 result = m_vMinXYZ;
		m_vMinXYZ = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		return result;
	}

	/********************************************************************************
	*	--- Static Function:
	*	GetInstance()
	*	This function will return the singleton instance of VertexBufferEngine
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ VertexBufferEngine*: the singleton instance
	********************************************************************************/
	static VertexBufferEngine* GetInstance();

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

	static VertexBufferEngine*		m_pInstance;		// singleton instance
	FILE*							pFile;		// pointer to file used by C style file I/O
	Vector3							m_vMaxXYZ;		// max position vector
	Vector3							m_vMinXYZ;		// min position vector
};

};
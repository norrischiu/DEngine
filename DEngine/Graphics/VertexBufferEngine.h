// Engine include
#include "D3D11Renderer.h"
#include "VertexFormat.h"
#include "Memory\Handle.h"

namespace DE
{

class VertexBufferEngine
{
public:
	VertexBufferEngine()
		:m_vMaxXYZ(-FLT_MAX, -FLT_MAX, -FLT_MAX)
		, m_vMinXYZ(FLT_MAX, FLT_MAX, FLT_MAX)
	{
	};

	ID3D11Buffer* CreateBufferFromRawData(void* pVertexData, const int iNumVerts, const unsigned int iDataSize, bool streamOut = false);
	void* CreateBuffer(const char* filename, int vertexFormat, unsigned int& stride);

	void FillVertexData_POSITION(const char* filename, unsigned int vertsNum, Handle hVertexData);

	void FillVertexData_POSITION_TEXTURE(const char* filename, unsigned int vertsNum, Handle hVertexData);

	void FillVertexData_POSITION_NORMAL_TEXTURE(const char* filename, unsigned int vertsNum, Handle hVertexData);

	void FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE(const char* filename, unsigned int vertsNum, Handle hVertexData);

	void FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE_FOUR_JOINTS(const char* filename, unsigned int vertsNum, Handle hVertexData);

	Vector3 GetMaxXYZ()
	{
		Vector3 result = m_vMaxXYZ;
		m_vMaxXYZ = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		return result;
	}

	Vector3 GetMinXYZ()
	{
		Vector3 result = m_vMinXYZ;
		m_vMinXYZ = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		return result;
	}

	static VertexBufferEngine* GetInstance();
	static void DestructandCleanUp();

private:
	static VertexBufferEngine*		m_pInstance;

	FILE*							pFile;

	Vector3							m_vMaxXYZ;
	Vector3							m_vMinXYZ;
};

};
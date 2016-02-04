#include "D3D11Renderer.h"
#include "VertexFormat.h"

class VertexBufferEngine
{
public:
	VertexBufferEngine() 
		:m_vMaxXYZ(FLT_MIN, FLT_MIN, FLT_MIN)
		,m_vMinXYZ(FLT_MAX, FLT_MAX, FLT_MAX)
	{
	};

	ID3D11Buffer* CreateBufferFromRawData(void* pVertexData, const int iNumVerts, const unsigned int iDataSize);
	void* CreateBuffer(const char* filename, int vertexFormat, unsigned int& stride);

	void FillVertexData_POSITION(const char* filename, unsigned int vertsNum, void* &pVertexData);

	void FillVertexData_POSITION_TEXTURE(const char* filename, unsigned int vertsNum, void* &pVertexData);

	void FillVertexData_POSITION_NORMAL_TEXTURE(const char* filename, unsigned int vertsNum, void* &pVertexData);

	void FillVertexData_POSITION_NORMAL_TANGENT_TEXTURE(const char* filename, unsigned int vertsNum, void* &pVertexData);

	Vector3 GetMaxXYZ()
	{
		Vector3 result = m_vMaxXYZ;
		m_vMaxXYZ = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);
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
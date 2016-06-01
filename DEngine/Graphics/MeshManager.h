// MeshManager.h: the class of a manager managing all mesh data

#include "MeshData.h"
#include "Utilities\MyHashMap.h"

namespace DE
{

// Singleton class of a mesh manager
class MeshManager
{
public:
	// Default empty constructor
	MeshManager() {}

	~MeshManager() 
	{
	}

	// Return singleton instance
	static MeshManager* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new MeshManager;
		return m_pInstance;
	}

	static void DestructandCleanUp() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

private:
	// The singleton instance
	static MeshManager*								m_pInstance;

	// Hash map of all mesh data
	MyHashMap<MeshData*>							m_MeshDataMap;
};

};
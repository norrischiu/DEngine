// MeshManager.h: the class of a manager managing all mesh data

#include <unordered_map>
#include "MeshData.h"

// Singleton class of a mesh manager
class MeshManager
{
public:
	// Default empty constructor
	MeshManager(){}

	// Construct the manager
	void Construct(){}

	// Destruct the manager, delete all mesh data
	void Destruct();

	// Return singleton instance
	static MeshManager* getInstance()
	{
		if (!m_pInstance)
			m_pInstance = new MeshManager;
		return m_pInstance;
	}

private:
	// The singleton instance
	static MeshManager*								m_pInstance;

	// Hash map of all mesh data
	std::unordered_map<unsigned int, MeshData*>		m_MeshDataMap;
};
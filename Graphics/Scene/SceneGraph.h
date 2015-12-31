#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_

#include <vector>
#include "Graphics/MeshComponent.h"

class SceneGraph
{
public:

	void Render();

	void RENDER_DEBUG_DRAWING();

	static SceneGraph* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new SceneGraph();
		return m_pInstance;
	}

	void AddComponent(MeshComponent* meshComponent);

	void ADD_DEBUG_DRAWING(MeshComponent* meshComponent);

	// Destructor
	~SceneGraph()
	{
		for (auto itr : m_tree)
		{
			delete itr;
		}
	}

private:

	// Singleton instance
	static SceneGraph*								m_pInstance;

	// Spatial data structure to store all render component
	std::vector<MeshComponent*>						m_tree; // temp

	// Temp storage for debug drawing
	std::vector<MeshComponent*>						DEBUG_DRAWING_TREE; // temp

};

#endif
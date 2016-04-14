#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_

#include <vector>
#include "Object\CameraComponent.h"
#include "Graphics\Render\VSPerObjectCBuffer.h"
#include "Graphics\Render\PSPerMaterialCBuffer.h"
#include "Graphics\Render\VSMatrixPaletteCBuffer.h"
#include "Graphics\Render\HSDSPerFrameCBuffer.h"

namespace DE
{

class MeshComponent;
class RenderPass;

class SceneGraph
{
public:

	SceneGraph();

	void FrustumCulling(Frustum frustum);

	void Render();

	void ShadowMapGeneration();

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
			//delete itr;
		}
		for (auto itr : DEBUG_DRAWING_TREE)
		{
			//delete itr;
		}
	}

private:

	// Singleton instance
	static SceneGraph*								m_pInstance;

	// Spatial data structure to store all render component
	std::vector<MeshComponent*>						m_tree; // temp

	// Temp storage for debug drawing
	std::vector<MeshComponent*>						DEBUG_DRAWING_TREE; // temp

	VSPerObjectCBuffer								m_VSCBuffer;

	PSPerMaterialCBuffer							m_PSCBuffer;

	HSDSPerFrameCBuffer								m_HSDSCBuffer;

	VSMatrixPaletteCBuffer							m_MatrixPalette;

	RenderPass*										m_ShadowPass;

};

};
#endif
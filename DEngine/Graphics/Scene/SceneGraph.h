#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_

// Engine include
#include "Object\CameraComponent.h"
#include "Graphics\Render\VSPerObjectCBuffer.h"
#include "Graphics\Render\PSPerMaterialCBuffer.h"
#include "Graphics\Render\VSMatrixPaletteCBuffer.h"
#include "Graphics\Render\HSDSPerFrameCBuffer.h"
#include "Utilities\MyArray.h"

namespace DE
{

// Engine include
class MeshComponent;
class RenderPass;

/*
*	CLASS: SceneGraph
*	SceneGraph is responsible rendering of a scene. Before rendering
*	it has to decide the visibility of meshes, currently using 
*	frustum culling. Shadow maps are also generated here
*/
class SceneGraph
{
public:

	/********************************************************************************
	*	--- Constructor:
	*	CBuffer(int, size_t);
	*	This constructor will construct the scene graph with any default render pass
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	SceneGraph();

	/********************************************************************************
	*	--- Destructor:
	*	CBuffer(int, size_t);
	*	This destructor will free the memory used by the mesh container
	********************************************************************************/
	~SceneGraph()
	{
	}

	/********************************************************************************
	*	--- Function:
	*	FrustumCulling(Frustum)
	*	This function will set the visibility of meshes according to a culling test
	*	against the given frustum
	*
	*	--- Parameters:
	*	@ frustum: the frustum to be tested against
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void FrustumCulling(Frustum frustum);

	/********************************************************************************
	*	--- Function:
	*	Render()
	*	This function will issue render command to GPU for all visibility meshes,
	*	and pass skinning matices for animated mesh
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void Render();

	/********************************************************************************
	*	--- Function:
	*	ShadowMapGeneration()
	*	This function will generate shadow maps for every shadow casting lights, and
	*	store to the light manager
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void ShadowMapGeneration();

	/********************************************************************************
	*	--- Static Function:
	*	GetInstance()
	*	This function will return the singleton instance of scene graph
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ SceneGraph*: the singleton instance
	********************************************************************************/
	static SceneGraph* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new SceneGraph();
		return m_pInstance;
	}

	/********************************************************************************
	*	AddComponent(MeshComponent*)
	*	This function will add a mesh component to the render tree
	*
	*	--- Parameters:
	*	@ meshComponent: the pointer to a mesh component to be added
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void AddComponent(MeshComponent* meshComponent);

	/********************************************************************************
	*	ADD_DEBUG_DRAWING(MeshComponent*)
	*	This function will add a debug mesh component to the debug render tree
	*
	*	--- Parameters:
	*	@ meshComponent: the pointer to a debug mesh component to be added
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	void ADD_DEBUG_DRAWING(MeshComponent* meshComponent);

private:


	static SceneGraph*								m_pInstance;	// Singleton instance
	MyArray<MeshComponent*>							m_tree;		// Spatial data structure to store all render component, currently a linear array only
	MyArray<MeshComponent*>							DEBUG_DRAWING_TREE;		// Temp storage for debug drawing
	VSPerObjectCBuffer								m_VSCBuffer;		// a default constant buffer interface for each object to be rendered
	VSMatrixPaletteCBuffer							m_MatrixPalette;	// a matrix palette constant buffer interface 
	HSDSPerFrameCBuffer								m_HSDSCBuffer;		// a default constant buffer interface for terrain rendering
	PSPerMaterialCBuffer							m_PSCBuffer;		// a default constant buffer interface for each material
	RenderPass*										m_ShadowPass;	// a shadow render pass
	ID3D12RootSignature*							m_pRootSignature; // root signature for D3D12 render
};

};
#endif
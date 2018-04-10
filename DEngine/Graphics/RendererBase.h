// RendererBase.h: the class for Direct3D 12 renderer
#ifndef RENDERER_BASE_H_
#define RENDERER_BASE_H_

// Windows include
#include <windows.h>

// Engine include
#include "GlobalInclude.h"
#include "Render\Texture.h"
#include "Object\CameraComponent.h"

namespace DE
{

class GBuffer;
class PostProcessEffect;

/*
*	CLASS: RendererBase
*	RendererBase is bass class renderer
*/
class RendererBase
{

public:

	const static unsigned int					RT_NUM = 2;		// Number of render target in G-Buffer

	/********************************************************************************
	*	--- Constructor:
	*	RendererBase()
	*	This constructor will do nothing, construction is deferred to construct
	*	function
	*
	*	--- Parameters:
	*	@ void
	********************************************************************************/
	RendererBase() {};

	/********************************************************************************
	*	--- Constructor:
	*	ConstructWithWindow(HWND)
	*	This function will create D3D12 device, context, swap chain, all render target
	*	gbuffer's texture and default states
	*
	*	--- Parameters:
	*	@ hWnd: the window instance
	*
	*	--- Return:
	*	@ bool: True if succeed, false if failed
	********************************************************************************/
	virtual bool ConstructWithWindow(HWND hWnd) = 0;

	/********************************************************************************
	*	--- Function:
	*	DestructandCleanUp()
	*	This function will release all renderer resources
	********************************************************************************/
	virtual void DestructandCleanUp() = 0;

	/********************************************************************************
	*	--- Function:
	*	Update(const float)
	*	This function will update any graphics related class, for example HUD
	*
	*	--- Parameters:
	*	@ deltaTime: the delta time of this frame
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	virtual void Update(const float deltaTime) = 0;

	/********************************************************************************
	*	--- Function:
	*	Render()
	*	This function will call render from all graphics related class
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	virtual void Render() = 0;

	/********************************************************************************
	*	--- Function:
	*	GetCamera()
	*	This function will return the pointer to the camera component using by the
	*	renderer
	*
	*	--- Parameters:
	*	@ void
	*
	*	--- Return:
	*	@ CameraComponent*: the pointer to the camera component
	********************************************************************************/
	virtual CameraComponent* GetCamera()
	{
		return m_RendererCamera;
	}

	/********************************************************************************
	*	--- Function:
	*	SetCamera(CameraComponent*)
	*	This function will set a camera component as the camera for rendering
	*
	*	--- Parameters:
	*	@ camera: the pointer to a camera component
	*
	*	--- Return:
	*	@ void
	********************************************************************************/
	virtual void SetCamera(CameraComponent* camera)
	{
		m_RendererCamera = camera;
	}

protected:

	CameraComponent*							m_RendererCamera;	// the pointer to current rendering camera
	GBuffer*									m_GBuffer;			// the pointer to GBuffer class
	PostProcessEffect*							m_PPE;				// the pointer to post-processing effect interface class
};

};
#endif
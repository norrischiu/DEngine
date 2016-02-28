// Component.h: abstract class for game object component to inherit from
#ifndef COMPONENT_H_
#define COMPONENT_H_

class GameObject;

namespace ComponentID
{
	enum
	{
		MESH,
		CAMERA,
		CONTROLLER,
		RIGIDBODY,
		POINTLIGHT,
		SPOTLIGHT,
		SKELETON,
		ANIMATION_CONTROLLER,
		ANIMATION_STATE_MACHINE
	};
}

class Component
{

public:

	// Default constructor
	Component()
	{
	};

	// Overload constructor: cache game object pointer
	Component(GameObject* ptr)
	{
		m_pOwner = ptr;
	}

	// Update in one frame
	virtual void Update(float deltaTime) = 0;

	// Set game object owner
	virtual void SetOwner(GameObject* ptr)
	{
		m_pOwner = ptr;
	}

	GameObject* GetOwner()
	{
		return m_pOwner;
	}

	inline int GetID()
	{
		return m_ID;
	}

protected:

	// Pointer to game object holding this component
	GameObject*						m_pOwner;

	// Cache delta time in update()
	float							m_fDeltaTime;

	int m_ID;
};


#endif // !COMPONENT_H
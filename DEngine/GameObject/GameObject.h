// GameObject.h: class for every entity in the game
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Math\simdmath.h"
#include "Object\Transform.h"
#include "Utilities\MyArray.h"

namespace DE
{

class GameWorld;
class Collide;

class GameObject
{
public:

	static int GameObjectID;

	GameObject();

	// Empty destructor: ownership at game world
	~GameObject() {}

	virtual void		Update(float deltaTime);

	void				collision(GameObject* gameObj);

	Collide*			getContact();

	bool				isCollided(GameObject* gameObj);

	void				SetTransform(const Matrix4 transform);

	void				TransformBy(const Matrix4& transform);

	void				AddComponent(Component* pComponent);

	// Return specific component pointer
	template<class T>
	T*			GetComponent()
	{
		const unsigned int size = m_vComponents.Size();
		for (int i = 0; i < size; ++i)
		{
			if (m_vComponents[i]->GetID() == T::ComponentID)
			{
				return (T*)m_vComponents[i];
			}
		}
		return nullptr;
	}

	inline Matrix4* GetTransform()
	{
		return m_pTransform->m_mWorldTransform;
	};

	inline Matrix4* GetLocalTransform()
	{
		return m_pTransform->m_mLocalTransform;
	}

	// Get world position directly
	Vector3 GetPosition()
	{
		return m_pTransform->m_mWorldTransform->GetPosition();
	}

	// Set world position directly
	void SetPosition(Vector3 vPos)
	{
		m_pTransform->m_mWorldTransform->SetPosition(vPos);
	}

protected:

	Collide*						m_pContact;

	// temp
	int								m_iGameObjectID;

	// Vector of all added components
	MyArray<Component*>				m_vComponents;

	// Default transform component
	Transform*						m_pTransform;

};

}; // namespace DE
#endif
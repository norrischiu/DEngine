// GameObject.h: class for every entity in the game
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Math\simdmath.h"
#include "Object\Transform.h"
#include "Physics\cdCollide.h"
#include <vector>

namespace DE
{

class GameWorld;

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

	std::vector<Component*>* getAllComponents();

	// Return specific component pointer
	template<class T>
	T*			GetComponent()
	{
		for (auto itr : m_vComponents)
		{
			if (itr->GetID() == T::ComponentID)
			{
				return (T*)itr;
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

	Vector3 GetPosition()
	{
		return m_pTransform->m_mWorldTransform->GetPosition();
	}

	int GetGameObjectID()
	{
		return m_iGameObjectID;
	}

	void SetPosition(Vector3 vPos)
	{
		Matrix4 trans;
		trans.CreateTranslation(vPos);
		*m_pTransform->m_mWorldTransform = trans;
	}

public:

	Collide*						m_pContact;

	// temp
	int								m_iGameObjectID;

	// Vector of all added components
	std::vector<Component*>			m_vComponents;

	// Default transform component
	Transform*						m_pTransform;

};

}; // namespace DE
#endif
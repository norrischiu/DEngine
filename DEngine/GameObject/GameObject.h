// GameObject.h: class for every entity in the game
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Math\simdmath.h"
#include "Physics\cdCollide.h"
#include "GameWorld.h"
#include <vector>

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

	void				Transform(const Matrix4& transform);

	void				AddComponent(Component* pComponent);

	// Return specific component pointer
	template<class T>
	T*			GetComponent()
	{
		for (auto itr : m_components)
		{
			if (itr->GetID() == T::ComponentID)
			{
				return (T*) itr;
			}
		}
		return nullptr;
	}

	inline Matrix4* GetTransform() 
	{
		return &m_mWorldTransform;
	};

	inline Matrix4* GetLocalTransform()
	{
		return &m_mLocalTransform;
	}

	Vector3 GetPosition()
	{
		return m_mWorldTransform.GetPosition();
	}

	void SetPosition(Vector3 vPos)
	{
		Matrix4 trans;
		trans.CreateTranslation(vPos);
		m_mWorldTransform = trans;
	}

	// hierarchical functionality
	void AttachTo(unsigned int objectID);

protected:

	Collide*					m_pContact;

	// Local to world transform
	Matrix4						m_mWorldTransform;

	// World to local transform
	Matrix4						m_mLocalTransform;

	// temp
	int							m_iGameObjectID;

	std::vector<Component*>		m_components;

	// hierarchical functionality
	int							m_parentID;
};

#endif
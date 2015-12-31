
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "..\Graphics\MeshComponent.h"
#include "..\Physics\cdObject.h"
#include "..\Math\simdmath.h"
#include "..\Physics\cdCollide.h"
#include "..\Physics\cdBody.h"
#include "GameWorld.h"

class GameObject
{
public:

	GameObject(Body* collObj, MeshComponent* meshObj, const Matrix4& transform, const int& gameObjID);

	// Empty destructor (ownership at game world)
	~GameObject() {}

	void		Update(float deltaTime);

	void		collision(const GameObject* gameObj);

	Collide*	getContact();

	Vector3		getTranslate();

	bool		isCollided(const GameObject* gameObj);

	void		setTransform(const Matrix4& transform);

	void		Transform(const Matrix4& transform);

	const Matrix4& GetTransform() 
	{
		return m_mWorldTransform;
	};

	const Matrix4& GetLocalTransform()
	{
		return m_mLocalTransform;
	}

	// hierarchical functionality
	void AttachTo(unsigned int objectID);

private:
	MeshComponent*			m_pMeshObj;
	Body*					m_pBody;
	Collide*				m_pContact;
	Matrix4					m_mWorldTransform;
	Matrix4					m_mLocalTransform;
	int						m_iGameObjectID;

	// hierarchical functionality
	int						m_parentID;
};





#endif
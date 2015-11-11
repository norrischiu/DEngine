
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "..\Graphics\MeshComponent.h"
#include "..\Physics\cdObject.h"
#include "..\Math\simdmath.h"
#include "..\Physics\cdCollide.h"
#include "..\Physics\cdBody.h"
#include "GameWorld.h"


typedef SIMDMatrix4 Matrix4;
typedef SIMDVector3 Vector3;

class GameObject
{
public:

	GameObject(Body* collObj, MeshComponent* meshObj, Collide* contact, const Matrix4& transform, const int& gameObjID);

	// Empty destructor (ownership at game world)
	~GameObject() {}

	void		Update(float deltaTime);

	void		collision(const GameObject* gameObj);

	Collide*	getContact();

	Vector3		getTranslate();
	void		objTranslate();

	bool		isCollided(const GameObject* gameObj);

	void		setTransform(const Matrix4& transform);

	void		Transform(const Matrix4& transform);

	const Matrix4& GetTransform() 
	{
		return m_Transform;
	};

	// Hiereachical functionality
	void AttachTo(unsigned int objectID);

	MeshComponent*			m_pMeshObj;

private:
	//MeshComponent*		m_pMeshObj;
	Body*					m_pBody;
	Collide*				m_pContact;
	SIMDMatrix4				m_Transform;
	int						m_GameObjectID;
	// Hiereachical functionality
	int						m_parentID;
};





#endif
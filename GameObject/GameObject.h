
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
// constructor
// create a game object with physics component and mesh component

// MeshComponent* meshObj = nullptr;
// ColliableObject* collObj = new ColliableObject(..);
// GameObject obj(Position/Transform, collObj, meshObj);
// debug drawing if necessary

class GameObject
{
public:
	GameObject(){}

	GameObject(Body* collObj, MeshComponent* meshObj, Collide* contact, const Matrix4& transform, const int& gameObjID);
	

	void		Update(float deltaTime);

	void		collision(const GameObject* gameObj);

	Collide*	getContact();

	Vector3		getTranslate();
	void		objTranslate();

	bool		isCollided(const GameObject* gameObj);

	void		setTransform(const Matrix4& transform);

private:
	MeshComponent*			m_pMeshObj;
	Body*					m_pBody;
	Collide*				m_pContact;
	SIMDMatrix4				m_Transform;
	int						m_GameObjectID;
};





#endif
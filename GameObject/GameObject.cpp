#include "GameObject.h"

GameObject::GameObject(Body * collObj, MeshComponent * meshObj, const Matrix4 & transform, const int& gameObjID)
{
	m_parentID =		-1;
	m_pBody	=			collObj;
	m_pMeshObj =		meshObj;
	m_pContact =		nullptr;
	m_mWorldTransform = transform;
	m_mLocalTransform =	Matrix4::Identity;
	m_pMeshObj->m_pTransform = &m_mWorldTransform; //
	m_iGameObjectID =	gameObjID;
	GameWorld::GetInstance()->AddGameObject(this);
}

void GameObject::Update(float deltaTime)
{
	m_mWorldTransform *= m_mLocalTransform;
	if (m_parentID != -1)
		m_mWorldTransform *= GameWorld::GetInstance()->GetGameObjectAt(m_parentID)->GetLocalTransform();
}

void GameObject::collision(const GameObject * gameObj)
{
	if (!m_pContact)
		m_pContact = new Collide();

	m_pContact->collision(this->m_pBody, gameObj->m_pBody);
}

Collide * GameObject::getContact()
{
	return m_pContact;
}

Vector3 GameObject::getTranslate()
{
	Vector3 translate(0.0f, 0.0f, 0.0f);
	translate.Transform(m_mWorldTransform);
	return translate;
}

bool GameObject::isCollided(const GameObject * gameObj)
{
	Collide contact;
	contact.collision(this->m_pBody, gameObj->m_pBody);
	return contact.getCollide();
}

void GameObject::setTransform(const Matrix4& transform)
{
	m_mWorldTransform = transform;
}

void GameObject::Transform(const Matrix4& transform)
{
	m_mLocalTransform *= transform;
}

void GameObject::AttachTo(unsigned int objectID)
{
	m_parentID = objectID;
}

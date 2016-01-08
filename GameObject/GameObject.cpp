#include "GameObject.h"

int GameObject::GameObjectID = 0;

GameObject::GameObject()
{
	m_parentID = -1;
	m_pContact = nullptr;
	m_mWorldTransform = Matrix4::Identity;
	m_mLocalTransform = Matrix4::Identity;
	m_iGameObjectID = GameObjectID++;
	GameWorld::GetInstance()->AddGameObject(this);
}

void GameObject::Update(float deltaTime)
{
	m_mWorldTransform *= m_mLocalTransform;
	// hierarchical functionality
	if (m_parentID != -1)
	{
		m_mWorldTransform *= *GameWorld::GetInstance()->GetGameObjectAt(m_parentID)->GetLocalTransform();
	}
	
	for (auto itr : m_components)
	{
		itr->Update(deltaTime);
	}
}

void GameObject::collision(GameObject * gameObj)
{
	if (!m_pContact)
		m_pContact = new Collide();

	m_pContact->collision(this->GetComponent<Body>(), gameObj->GetComponent<Body>());
}

Collide * GameObject::getContact()
{
	return m_pContact;
}

bool GameObject::isCollided(GameObject * gameObj)
{
	Collide contact;
	contact.collision(this->GetComponent<Body>(), gameObj->GetComponent<Body>());
	return contact.getCollide();
}

void GameObject::setTransform(const Matrix4& transform)
{
	m_mWorldTransform = transform;
}

void GameObject::Transform(const Matrix4& transform)
{
	m_mWorldTransform *= transform;
}

void GameObject::AddComponent(Component * pComponent)
{
	pComponent->SetOwner(this);
	m_components.push_back(pComponent);
}

void GameObject::AttachTo(unsigned int objectID)
{
	m_parentID = objectID;
}

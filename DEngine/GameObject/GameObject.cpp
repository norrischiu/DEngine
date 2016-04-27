#include "GameObject.h"
#include "GameWorld.h"
#include "Object\Transform.h"

namespace DE
{

int GameObject::GameObjectID = 0;

GameObject::GameObject()
{
	DE::Handle hTransform(sizeof(Transform));
	new (hTransform) Transform();
	AddComponent((DE::Component*) hTransform.Raw());
	m_pTransform = (DE::Transform*) hTransform.Raw();

	m_pContact = nullptr;
	m_iGameObjectID = GameObjectID++;
	GameWorld::GetInstance()->AddGameObject(this);
}

void GameObject::Update(float deltaTime)
{
	for (auto itr : m_vComponents)
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

void GameObject::SetTransform(const Matrix4 transform)
{
	*m_pTransform->m_mWorldTransform = transform;
}

void GameObject::TransformBy(const Matrix4& transform)
{
	*m_pTransform->m_mWorldTransform *= transform;
}

void GameObject::AddComponent(Component * pComponent)
{
	pComponent->SetOwner(this);
	m_vComponents.push_back(pComponent);
}


}; // namespace DE

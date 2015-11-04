#include "cdObject.h"


CollidableObject::CollidableObject(Body * body, const Vector3& translate, const int objectID)
{
	m_pBody = body;
	m_ObjectID = objectID;
	m_Translate = translate;
	//CollisionWorld::GetInstance()->getObjectList().push_back(this);
}

Body * CollidableObject::getBody() const
{
	return m_pBody;
}

void CollidableObject::translate(Vector3& translate)
{
	m_pBody->update(0.0f, translate);
}

void CollidableObject::update()
{

}

//gameobjectarray[ID]->getTransform

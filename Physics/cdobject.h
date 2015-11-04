
#ifndef CDOBJECT_H
#define CDOBJECT_H

#include "cdBody.h"
#include "..\Math\simdmath.h"
#include "..\Physics\cdCollisionWorld.h"

typedef SIMDVector3 Vector3;
typedef SIMDMatrix4 Matrix4;

class CollidableObject
{
public:
	CollidableObject(){}

	CollidableObject(Body* body, const Vector3& translate, const int objectID);

	int getObjectID() const { return m_ObjectID; }
	Body* getBody() const;
	void translate(Vector3& translate);
	Vector3 getPosition() const { return m_pBody->getCenter(); }

	void update();

private:
	Body*			m_pBody;
	Vector3			m_Translate;
	int				m_ObjectID;

	// int EntityIO;
};




#endif
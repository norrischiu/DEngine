
#ifndef CDBODY_H
#define CDBODY_H

#include "Object\Component.h"
#include "cdCollide.h"
#include "Math/simdmath.h"

namespace DE
{

enum
{
	typeSPHERE,
	typeAABB,
	typePOINT,
	typeRAY,
	typePLANE,

	typeCount
};

class Body : public Component
{
public:

	static const int ComponentID = ComponentID::RIGIDBODY;

	Body()
	{
		m_Type = -1;
		m_ID = ComponentID;
	}

	Body(const int type)
	{
		m_Type = type;
		m_ID = ComponentID;
	}

	int getType() const { return m_Type; }
	void setType(const int type) { m_Type = type; }
	Vector3 getCenter() const;
	virtual void computeAABB(const Matrix4& transform) {}

	virtual void update(const float deltaTime, const Vector3& translate) {}

	// Inherited via Component
	void Update(float deltaTime) override;

private:
	int m_Type;

};

};

#endif
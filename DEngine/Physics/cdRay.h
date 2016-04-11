
#ifndef CDRAY_H
#define CDRAY_H

#include "../Math/simdmath.h"
#include "cdCollide.h"
#include "cdBody.h"

namespace DE
{

class Ray : public Body
{
public:
	Ray() {}

	Ray(const Vector3& dir, const Vector3& start)
	{
		m_Dir = dir;
		m_Start = start;
		Body::setType(typeRAY);
	}

	Vector3 getDir() const { return m_Dir; }
	Vector3 getStart() const { return m_Start; }
	virtual void update(const float deltaTime, const Vector3& translate);

	// Inherited via Body
	virtual void Transform(Matrix4 & transform) override;

private:
	Vector3 m_Dir;
	Vector3 m_Start;
};

};
#endif
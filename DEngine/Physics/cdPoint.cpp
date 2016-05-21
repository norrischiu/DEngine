#include "cdPoint.h"

namespace DE
{

void Point::update(const float deltaTime, const Vector3 & translate)
{
	Vector3 tran = translate;
	tran.Multiply(deltaTime);
	m_Point += tran;
}

const Vector3 Point::getCenter()
{
	return m_Point;
}

void Point::Transform(Matrix4 & transform)
{
	//TODO
}

};
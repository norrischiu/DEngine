#include "cdSphere.h"

namespace DE
{

void Sphere::update(const float deltaTime, const Vector3 & translate)
{
	Vector3 tran = translate;
	tran.Multiply(deltaTime);
	m_Center += tran;
}

void Sphere::Transform(Matrix4 & transform)
{
	m_Center.Transform(transform);
}

};
#include "cdSphere.h"

namespace DE
{

void Sphere::update(const float deltaTime, const Vector3 & translate)
{
	Vector3 tran = translate;
	tran.Multiply(deltaTime);
	m_Center += tran;
}

};
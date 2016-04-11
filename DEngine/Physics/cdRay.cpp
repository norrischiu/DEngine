#include "cdRay.h"

namespace DE
{

void Ray::update(const float deltaTime, const Vector3 & translate)
{
	Vector3 tran = translate;
	tran.Multiply(deltaTime);
	m_Start += tran;
}

void Ray::Transform(Matrix4 & transform)
{
	m_Dir.TransformAsVector(transform);
	m_Start.Transform(transform);
}

};
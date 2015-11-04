
#include "cdCollide.h"
#include "cdSphere.h"
#include "cdBody.h"
#include "cdAabb.h"
#include "cdPoint.h"
#include "cdRay.h"
#include <math.h>


void Collide::setResponseObject1(const Vector3& response)
{
	m_ResponseObject1.m_pObjectResponse = response;
}

void Collide::setResponseObject2(const Vector3& response)
{
	m_ResponseObject2.m_pObjectResponse = response;
}

void Collide::collision(const Body * body1, const Body * body2)
{
	if (body1->getType() == typeAABB && body2->getType() == typeAABB)
		boxBoxCollide(body1, body2);
	else if (body1->getType() == typeSPHERE && body2->getType() == typeSPHERE)
		sphereSphereCollide(body1, body2);
	// box vs sphere OR sphere vs box
	else if (body1->getType() == typeAABB && body2->getType() == typeSPHERE)
		boxSphereCollide(body1, body2);
	else if (body1->getType() == typeSPHERE && body2->getType() == typeAABB)
		boxSphereCollide(body2, body1);
	// point vs box OR box vs point
	else if (body1->getType() == typePOINT && body2->getType() == typeAABB)
		pointBoxCollide(body1, body2);
	else if (body1->getType() == typeAABB && body2->getType() == typePOINT)
		pointBoxCollide(body2, body1);
	// point vs sphere OR sphere vs point
	else if (body1->getType() == typePOINT && body2->getType() == typeSPHERE)
		pointSphereCollide(body1, body2);
	else if (body1->getType() == typeSPHERE && body2->getType() == typePOINT)
		pointSphereCollide(body2, body1);
	// ray vs sphere OR sphere vs ray
	else if (body1->getType() == typeRAY && body2->getType() == typeSPHERE)
		raySphereCollide(body1, body2);
	else if (body1->getType() == typeSPHERE && body2->getType() == typeRAY)
		raySphereCollide(body2, body1);
	// ray vs box OR box vs ray
	else if (body1->getType() == typeRAY && body2->getType() == typeAABB)
		rayBoxCollide(body1, body2);
	else if (body1->getType() == typeAABB && body2->getType() == typeRAY)
		rayBoxCollide(body2, body1);
	else
		printf("no match object type!");

}


void Collide::boxBoxCollide(const Body * box1, const Body * box2)
{
	AABB *aabb1 = (AABB*)box1;
	AABB *aabb2 = (AABB*)box2;
//	float maxDistance = 0.0f;

	bool collide = false;
	// max > min min< max
	if (aabb1->getMax().GetX() < aabb2->getMin().GetX() &&
		aabb1->getMin().GetX() > aabb2->getMax().GetX() &&
		aabb1->getMax().GetY() > aabb2->getMin().GetY() &&
		aabb1->getMin().GetY() < aabb2->getMax().GetY() &&
		aabb1->getMax().GetZ() < aabb2->getMin().GetZ() &&
		aabb1->getMin().GetZ() > aabb2->getMax().GetZ())
		collide = true;

	setCollide(collide);
	setDistance(0.0f);
/**
	SIMDVector3 dist1 = aabb2->getMin() - aabb1->getMax();
	SIMDVector3 dist2 = aabb1->getMin() - aabb2->getMax();

	// create the float array to store the maximum X, Y and Z
	float max[3];
	// create a float to store the maximum float among the
	// values of float array max[3]
	maxDistance = max[0];

	// here is the calculation to generate max[3]
	if (dist1.GetX() > dist2.GetX())
		max[0] = dist1.GetX();
	else
		max[0] = dist2.GetX();
	if (dist1.GetY() > dist2.GetY())
		max[1] = dist1.GetY();
	else
		max[1] = dist1.GetY();
	if (dist1.GetZ() > dist2.GetZ())
		max[2] = dist1.GetZ();
	else
		max[2] = dist1.GetZ();

	// pick up the maximum value from max[3]
	for (int i = 1; i < 3; i++)
	{
		if (maxDistance < max[i])
			maxDistance = max[i];
	}

	setCollide(maxDistance < 0);
	setDistance(maxDistance);
*/
	// compute the response vectors
	Vector3 responseObject1 = box1->getCenter() - box2->getCenter();
	Vector3 responseObject2 = box2->getCenter() - box1->getCenter();
	setResponseObject1(responseObject1.Normalize());
	setResponseObject2(responseObject2.Normalize());
}

void Collide::sphereSphereCollide(const Body * sphere_1, const Body * sphere_2)
{
	Sphere *sphere1 = (Sphere*)sphere_1;
	Sphere *sphere2 = (Sphere*)sphere_2;
	float realDist = 0.0f;
	float radiusDist = sphere1->getRadius() + sphere2->getRadius();
	float centerDist = (sphere1->getCenter() - sphere2->getCenter()).Length();
	realDist = centerDist - radiusDist;
	setCollide(realDist < 0);
	setDistance(realDist);

	// compute the response vectors
	Vector3 responseObject1 = sphere_1->getCenter() - sphere_2->getCenter();
	Vector3 responseObject2 = sphere_2->getCenter() - sphere_1->getCenter();
	setResponseObject1(responseObject1.Normalize());
	setResponseObject2(responseObject2.Normalize());
}

void Collide::boxSphereCollide(const Body * box, const Body * sphere)
{
	AABB* m_box = (AABB*)box;
	Sphere* m_sphere = (Sphere*)sphere;
	float dMin = 0.0f;

	Vector3 sCenter = m_sphere->getCenter();
	Vector3 bMin = m_box->getMin();
	Vector3 bMax = m_box->getMax();

	if (sCenter.GetX() > bMin.GetX())
		dMin += pow(sCenter.GetX() - bMin.GetX(), 2);
	else if (sCenter.GetX() < bMax.GetX())
		dMin += pow(sCenter.GetX() - bMax.GetX(), 2);

	if (sCenter.GetY() < bMin.GetY())
		dMin += pow(sCenter.GetY() - bMin.GetY(), 2);
	else if (sCenter.GetY() > bMax.GetY())
		dMin += pow(sCenter.GetY() - bMax.GetY(), 2);

	if (sCenter.GetZ() < bMin.GetZ())
		dMin += pow(sCenter.GetZ() - bMin.GetZ(), 2);
	else if (sCenter.GetY() > bMax.GetY())
		dMin += pow(sCenter.GetZ() - bMax.GetZ(), 2);

	if (dMin <= pow(m_sphere->getRadius(), 2))
	{
		setCollide(true);
		setDistance(1);
	}
	else
	{
		setCollide(false);
		setDistance(-1);
	}
}

void Collide::pointBoxCollide(const Body * point_, const Body * box)
{
	Point *point = (Point*)point_;
	AABB *aabb = (AABB*)box;

	if (point->getPoint().GetX() > aabb->getMin().GetX() && point->getPoint().GetX() < aabb->getMax().GetX() &&
		point->getPoint().GetY() > aabb->getMin().GetY() && point->getPoint().GetY() < aabb->getMax().GetY() &&
		point->getPoint().GetZ() > aabb->getMin().GetZ() && point->getPoint().GetZ() < aabb->getMax().GetZ())
	{
		setCollide(true);
		// haven't calculate the distance
		setDistance(-1);
	}
	else
	{
		setCollide(false);
		// haven't calculate the distance
		setDistance(1);
	}

	// compute the response vectors
	Vector3 responseObject1 = point_->getCenter() - box->getCenter();
	Vector3 responseObject2 = box->getCenter() - point_->getCenter();
	setResponseObject1(responseObject1);
	setResponseObject2(responseObject2);
}

void Collide::pointSphereCollide(const Body * point_, const Body * sphere_)
{
	Point *point = (Point*)point_;
	Sphere *sphere = (Sphere*)sphere_;

	float distance = 0.0f;
	distance = (point->getPoint() - sphere->getCenter()).Length();

	setCollide(distance < sphere->getRadius());
	setDistance(distance - sphere->getRadius());

	// compute the response vectors
	Vector3 responseObject1 = point_->getCenter() - sphere_->getCenter();
	Vector3 responseObject2 = sphere_->getCenter() - point_->getCenter();
	setResponseObject1(responseObject1);
	setResponseObject2(responseObject2);
}

void Collide::raySphereCollide(const Body * ray_, const Body * sphere_)
{
	Ray *ray = (Ray*)ray_;
	Sphere *sphere = (Sphere*)sphere_;
	Vector3 vec = ray->getStart() - sphere->getCenter();
	setCollide(true);
	setDistance(0.0f);
	float fB = vec.Dot(ray->getDir());
	float fC = vec.Dot(vec) - sphere->getRadius()*sphere->getRadius();
	if (fC > 0.0f && fB > 0.0f)
		setCollide(false);
	float fDisc = fB*fB - fC;
	if (fDisc < 0.0f)
		setCollide(false);

	// compute the response vectors
	Vector3 responseObject1 = ray_->getCenter()- sphere_->getCenter();
	Vector3 responseObject2 = sphere_->getCenter() - ray_->getCenter();
	setResponseObject1(responseObject1);
	setResponseObject2(responseObject2);
}

void Collide::rayBoxCollide(const Body * ray, const Body * box)
{
}

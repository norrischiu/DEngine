
#ifndef CDCOLLIDE_H
#define CDCOLLIDE_H

#include "../Math/simdmath.h"
#include "cdObject.h"
#include "cdBody.h"

class Body;
typedef SIMDVector3 Vector3;
typedef SIMDMatrix4 Matrix4;

struct Response
{
	Vector3 m_pObjectResponse;
	int m_pObjectID;
};

class Collide
{
public:

	Collide(){}

	// two basic getters
	const bool  getCollide() const { return m_Collide; }
	const float getDistance() const { return m_Distance; }
	const Response getResponseObject1() const { return m_ResponseObject1; }
	const Response getResponseObject2() const { return m_ResponseObject2; }
	void setCollide(bool collide) { m_Collide = collide; }
	void setDistance(float distance) { m_Distance = distance; }
	void setResponseObject1(const Vector3& response);
	void setResponseObject2(const Vector3& response);


	// handle collision detection
	void collision(const Body* object1, const Body* object2);
	void boxBoxCollide(const Body* box1, const Body* box2);
	void sphereSphereCollide(const Body* sphere1, const Body* sphere2);
	void boxSphereCollide(const Body* box, const Body* sphere);
	void pointBoxCollide(const Body* point, const Body* box);
	void pointSphereCollide(const Body* point, const Body* sphere);
	void raySphereCollide(const Body* ray, const Body* sphere);
	void rayBoxCollide(const Body* ray, const Body* box);


private:
	// if collide, return true; otherwise, return false
	bool				m_Collide;
	// if two bodies collide, return the distance of the overlap betwenn two spheres
	float				m_Distance;
	// response of object1 after collided
	Response			m_ResponseObject1;
	// response of object2 after collided
	Response			m_ResponseObject2;
};


#endif
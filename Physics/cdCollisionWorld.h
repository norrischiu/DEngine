#ifndef CDCOLLISIONWORLD_H
#define CDCOLLISIONWORLD_H
#include <vector>
#include "cdObject.h"
#include "cdCollide.h"
class CollidableObject;

#pragma once

class CollisionWorld
{
public:
	

	CollisionWorld() {}

	CollisionWorld* GetInstance();

	std::vector<CollidableObject*>& getObjectList();
	

private:
	CollisionWorld*						m_pInstance;
	std::vector<CollidableObject*>		m_ObjectList;
	
};


#endif
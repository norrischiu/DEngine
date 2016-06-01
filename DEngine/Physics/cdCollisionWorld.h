#ifndef CDCOLLISIONWORLD_H
#define CDCOLLISIONWORLD_H

#include "cdObject.h"
#include "cdCollide.h"
class CollidableObject;

#pragma once

class CollisionWorld
{
public:
	

	CollisionWorld() {}

	CollisionWorld* GetInstance();

private:
	CollisionWorld*						m_pInstance;
	//std::vector<CollidableObject*>		m_ObjectList;
	
};


#endif
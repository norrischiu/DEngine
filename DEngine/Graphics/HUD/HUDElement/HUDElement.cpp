#include "HUDElement.h"



HUDElement::HUDElement(char* id, const HUDElement::Position pos) : m_id(id), m_pos(pos), m_hasUpdate(false)
{
}

char* HUDElement::getID()
{
	return m_id;
}

bool HUDElement::hasUpdate()
{
	return m_hasUpdate;
}

void HUDElement::setHasUpdate(const bool hasUpdate)
{
	m_hasUpdate = hasUpdate;
}

HUDElement::Position HUDElement::getPosition()
{
	return m_pos;
}

void HUDElement::setPosition(const HUDElement::Position pos)
{
	if (pos.x >= 0 && pos.x <= 1024 && 
		pos.y >= 0 && pos.y <= 768
	){
		m_pos = pos;
		m_hasUpdate = true;
	}
}

HUDElement::~HUDElement()
{
}

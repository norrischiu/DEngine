#include "HUDElement.h"

namespace DE
{

HUDElement::HUDElement(char* id, const HUDElement::Position pos, const float duration, const bool visibility) :
	m_id(id), m_pos(pos), m_hasUpdate(false), m_duration(duration), m_visibility(visibility)
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
		) {
		m_pos = pos;
		m_hasUpdate = true;
	}
}

float HUDElement::getDuration()
{
	return m_duration;
}

void HUDElement::setDuration(const float duration)
{
	m_duration = duration;
}

bool HUDElement::isVisible()
{
	return m_visibility;
}

void HUDElement::setVisibility(const bool visibility)
{
	m_visibility = visibility;
}

HUDElement::~HUDElement()
{
}

};
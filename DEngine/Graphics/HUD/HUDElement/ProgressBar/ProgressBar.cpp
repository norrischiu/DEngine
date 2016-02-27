#include "ProgressBar.h"

ProgressBar::ProgressBar(char* id, const float progress, const HUDElement::Position pos, const HUDElement::Size size, const bool showText)
	: HUDElement(id, pos), m_size(size), m_showText(showText)
{
	if (progress >= 0.0f && progress <= 100.0f) {
		m_progress = progress;
	} else {
		m_progress = 0.0f;
	}
}


ProgressBar::~ProgressBar()
{
}

float ProgressBar::getProgress()
{
	return m_progress;
}

void ProgressBar::setProgress(const float progress)
{
	if (progress >= 0.0f && progress <= 100.0f)
	{
		m_progress = progress;
		HUDElement::setHasUpdate(true);
	}
}

HUDElement::Size ProgressBar::getSize()
{
	return m_size;
}

void ProgressBar::setSize(const HUDElement::Size size)
{
	m_size = size;
	HUDElement::setHasUpdate(true);
}

bool ProgressBar::isShowText()
{
	return m_showText;
}

void ProgressBar::setShowText(const bool showText)
{
	m_showText = showText;
	HUDElement::setHasUpdate(true);
}

int ProgressBar::getTypeID()
{
	return typeID;
}

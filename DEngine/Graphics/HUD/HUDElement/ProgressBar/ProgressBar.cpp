#include "ProgressBar.h"

ProgressBar::ProgressBar(char* id, const float progress, const HUDElement::Position pos, const HUDElement::Size size, const bool showText)
	: HUDElement(id, pos), m_progress(progress), m_size(size), m_showText(showText)
{
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
	m_progress = progress;
	HUDElement::setHasUpdate(true);
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

#include "ProgressBar.h"

ProgressBar::ProgressBar(char* id, const float progress, const HUDElement::Position pos, const HUDElement::Size size, const bool showText, const float duration)
	: HUDElement(id, pos, duration), m_size(size), m_showText(showText)
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

ProgressBar* ProgressBar::setProgress(const float progress)
{
	if (progress >= 0.0f && progress <= 100.0f)
	{
		m_progress = progress;
		HUDElement::setHasUpdate(true);
	}

	return this;
}

HUDElement::Size ProgressBar::getSize()
{
	return m_size;
}

ProgressBar* ProgressBar::setSize(const HUDElement::Size size)
{
	m_size = size;
	HUDElement::setHasUpdate(true);
	return this;
}

bool ProgressBar::isShowText()
{
	return m_showText;
}

ProgressBar* ProgressBar::setShowText(const bool showText)
{
	m_showText = showText;
	HUDElement::setHasUpdate(true);
	return this;
}

int ProgressBar::getTypeID()
{
	return typeID;
}

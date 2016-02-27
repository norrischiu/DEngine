#pragma once
#include "../HUDElement.h"

class ProgressBar : public HUDElement
{
	
public:
	static const int typeID = HUDElement::TypeID::PROGRESSBAR;

	ProgressBar(char* m_id, const float progress, const HUDElement::Position, const HUDElement::Size, const bool showText, const float duration = -1.0f);
	~ProgressBar();

	float getProgress();
	void setProgress(const float progress);
	HUDElement::Size getSize();
	void setSize(const HUDElement::Size size);
	bool isShowText();
	void setShowText(const bool showText);
	int getTypeID();

private:
	float m_progress;
	HUDElement::Size m_size;
	bool m_showText;
};


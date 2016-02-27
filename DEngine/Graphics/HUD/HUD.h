#pragma once

#include <unordered_map>
#include "Math/simdmath.h"
#include "HUDElement\TextBox\TextBox.h"
#include "HUDElement\ProgressBar\ProgressBar.h"
#include "Graphics\Render\VSPerObjectCBuffer.h"

class HUD
{
public:
	HUD();
	~HUD();

	static HUD* getInstance();

	TextBox* addText(char* id, char* string, const HUDElement::Position pos, const HUDElement::FontSize fontSizePt, const HUDElement::Color color, const float duration = -1.0f);
	ProgressBar* addProgress(char* id, const float progress, const HUDElement::Position pos, const HUDElement::Size size, const bool showText, const float duration = -1.0f);

	HUDElement* getHUDElementById(const char* id);
	void removeHUDElementById(const char* id);
	void cleanUpCache(const char* id);
	void update(const float delta_time);
	void Render();
	void destructAndCleanUp();

private:
	std::unordered_map<const char*, HUDElement*> m_elements;
	std::unordered_map<const char*, float> m_timer;
	static HUD* m_instance;

	VSPerObjectCBuffer*	m_pVSCBuffer;
};


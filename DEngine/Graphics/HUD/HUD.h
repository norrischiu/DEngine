#pragma once

#include "Math/simdmath.h"
#include "HUDElement\TextBox\TextBox.h"
#include "HUDElement\ProgressBar\ProgressBar.h"
#include "Graphics\Render\VSPerObjectCBuffer.h"
#include "Utilities\MyHashMap.h"

namespace DE
{

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
	MyHashMap<HUDElement*> m_elements;
	MyHashMap<float> m_timer;
	static HUD* m_instance;

	VSPerObjectCBuffer*	m_pVSCBuffer;
};

};
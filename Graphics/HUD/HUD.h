#pragma once

#include <unordered_map>
#include "Math/simdmath.h"
#include "HUDElement\TextBox\TextBox.h"
#include "HUDElement\ProgressBar\ProgressBar.h"
#include "Render\VSPerObjectCBuffer.h"

class HUD
{
public:
	HUD();
	~HUD();

	static HUD* getInstance();

	void addText(char* id, char* string, const HUDElement::Position pos, const TextBox::FontSize fontSizePt, const HUDElement::Color color);
	void addProgress(char* id, const float progress, const HUDElement::Position pos, const HUDElement::Size size, const bool showText = true);

	HUDElement* getHUDElementById(const char* id);
	void removeHUDElementById(const char* id);
	void destructAndCleanUp();
	void Render();

private:
	std::unordered_map<const char*, HUDElement*> m_elements;
	static HUD* m_instance;

	VSPerObjectCBuffer*	m_pVSCBuffer;
};


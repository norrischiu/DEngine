#pragma once

#include <unordered_map>
#include <string>
#include "../HUDElement.h"
#include "Math/simdmath.h"

namespace DE
{

class TextBox : public HUDElement
{

public:
	static const int typeID = HUDElement::TypeID::TEXTBOX;

	TextBox(char* id, char* string, const HUDElement::Position pos, const int fontSizePt, const HUDElement::Color fontColor, const float duration = -1.0f);
	~TextBox();

	int getTypeID();
	const char* getText();
	int getFontSize();
	int getColor();
	TextBox* setText(char* string);
	TextBox* setText(const char* format, ...);
	TextBox* setFontSize(const int fontSize);
	TextBox* setColor(const HUDElement::Color color);

private:
	std::string m_text;
	int m_fontSizePt;
	int m_color;
};

};
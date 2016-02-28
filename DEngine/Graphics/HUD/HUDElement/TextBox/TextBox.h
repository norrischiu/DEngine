#pragma once

#include <unordered_map>
#include "../HUDElement.h"
#include "Math/simdmath.h"

class TextBox : public HUDElement
{

public:
	static const int typeID = HUDElement::TypeID::TEXTBOX;

	TextBox(char* id, char* string, const HUDElement::Position pos, const int fontSizePt, const HUDElement::Color fontColor, const float duration = -1.0f);
	~TextBox();

	int getTypeID();
	char* getText();
	int getFontSize();
	int getColor();
	TextBox* setText(char* string);
	TextBox* setFontSize(const int fontSize);
	TextBox* setColor(const HUDElement::Color color);

private:
	char* m_prevText;
	char* m_text;
	int m_fontSizePt;
	int m_color;
};

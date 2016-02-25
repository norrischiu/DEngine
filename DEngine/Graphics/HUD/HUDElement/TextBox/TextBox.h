#pragma once

#include <unordered_map>
#include "../HUDElement.h"
#include "Math/simdmath.h"

class TextBox : public HUDElement
{

public:
	static const int typeID = HUDElement::TypeID::TEXTBOX;

	TextBox(char* id, char* string, const HUDElement::Position pos, const int fontSizePt, const HUDElement::Color fontColor);
	~TextBox();

	int getTypeID();
	const char* getText();
	int getFontSize();
	int getColor();
	void setText(char* string);
	void setFontSize(const int fontSize);
	void setColor(const HUDElement::Color color);

private:
	char* m_prevText;
	char* m_text;
	int m_fontSizePt;
	int m_color;
};


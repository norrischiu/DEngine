#pragma once

#include <unordered_map>
#include "../HUDElement.h"
#include "Math/simdmath.h"

class TextBox : public HUDElement
{

public:
	enum FontSize {
		PT12 = 12, PT24 = 24, PT36 = 36, PT48 = 48, PT60 = 60, PT72 = 72, PT84 = 84, PT96 = 96, PT108 = 108, PT120 = 120, PT132 = 132, PT144 = 144, PT156 = 156, PT168 = 168, PT180 = 180, PT192 = 192, PT204 = 204
	};

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


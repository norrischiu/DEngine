#include "TextBox.h"

TextBox::TextBox(char* id, char* string, const HUDElement::Position pos, const int fontSizePt, const HUDElement::Color fontColor, const float duration) :
	m_text(string), m_fontSizePt(fontSizePt), m_color(fontColor), HUDElement(id, pos, duration)
{

}

int TextBox::getTypeID()
{
	return typeID;
}

const char* TextBox::getText()
{
	return m_text;
}

void TextBox::setText(char* string)
{
	m_text = string;
	HUDElement::setHasUpdate(true);
}


void TextBox::setFontSize(const int fontSizePt)
{
	m_fontSizePt = fontSizePt;
	HUDElement::setHasUpdate(true);
}

void TextBox::setColor(const HUDElement::Color color)
{
	m_color = color;
	HUDElement::setHasUpdate(true);
}

int TextBox::getColor()
{
	return m_color;
}

int TextBox::getFontSize()
{
	return m_fontSizePt;
}

TextBox::~TextBox()
{

}

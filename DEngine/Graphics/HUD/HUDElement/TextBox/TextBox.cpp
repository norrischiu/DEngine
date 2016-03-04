#include "TextBox.h"
#include <cstdarg>

namespace DE
{

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
	return m_text.c_str();
}

TextBox* TextBox::setText(char* string)
{
	m_text = string;
	HUDElement::setHasUpdate(true);
	return this;
}

TextBox* TextBox::setText(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char string[128];
	vsnprintf(string, sizeof(string), format, args);
	va_end(args);
	return setText(string);
}

int TextBox::getFontSize()
{
	return m_fontSizePt;
}

TextBox* TextBox::setFontSize(const int fontSizePt)
{
	m_fontSizePt = fontSizePt;
	HUDElement::setHasUpdate(true);
	return this;
}

int TextBox::getColor()
{
	return m_color;
}

TextBox* TextBox::setColor(const HUDElement::Color color)
{
	m_color = color;
	HUDElement::setHasUpdate(true);
	return this;
}

TextBox::~TextBox()
{

}

};
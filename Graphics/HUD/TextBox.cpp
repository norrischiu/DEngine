#include "TextBox.h"

TextBox::TextBox(const char* string, const Vector3& pos, const int color) : 
	m_text(string), m_pos(pos), m_color(color)
{

}

const char* TextBox::getText()
{
	return m_text.c_str();
}

TextBox* TextBox::setText(const char* string)
{
	m_text = string;
	return this;
}

TextBox* TextBox::setPosition(const Vector3& pos)
{
	m_pos = pos;
	return this;
}

TextBox* TextBox::setColor(const int color)
{
	m_color = color;
	return this;
}

TextBox::~TextBox()
{

}

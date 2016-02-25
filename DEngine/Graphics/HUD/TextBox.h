#pragma once

#include <unordered_map>
#include "Math/simdmath.h"

class TextBox
{

public:
	TextBox(const char* string, const Vector3& pos, const int color);
	~TextBox();

	const char* getText();
	TextBox* setText(const char* string);
	TextBox* setPosition(const Vector3& pos);
	TextBox* setColor(const int color);

private:
	std::string m_text;
	Vector3 m_pos;
	int m_color;
};


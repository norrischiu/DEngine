#pragma once

#include <unordered_map>
#include "Math/simdmath.h"
#include "TextBox.h"

class HUD
{
public:
	HUD();
	~HUD();

	void addText(const char* string, const char* id, const Vector3& pos, const int color);
	void removeTextById(const char* id);
	void renderHUD();

private:
	std::unordered_map<const char*, TextBox> m_textBoxes;
};


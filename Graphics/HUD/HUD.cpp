#include "HUD.h"
#include "TextEngine.h"

HUD::HUD()
{
}

void HUD::addText(const char* string, const char* id, const Vector3& pos, const int color)
{
	TextBox textBox(string, pos, color);
	std::pair<const char*, TextBox> pair(id, textBox);
	m_textBoxes.insert(pair);
}

void HUD::removeTextById(const char* id)
{
	m_textBoxes.erase(id);
}

void HUD::renderHUD()
{
	for (auto l : m_textBoxes) {
		TextEngine::getInstance()->makeText(l.second)->Draw();
	}
}

HUD::~HUD()
{
}

#include "TextEngine.h"

TextEngine* TextEngine::m_instance;

TextEngine::TextEngine()
{

}

TextEngine* TextEngine::getInstance()
{
	if (!m_instance) {
		m_instance = new TextEngine();
	}

	return m_instance;
}

void TextEngine::destructAndCleanUp()
{
	if (m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

MeshComponent* TextEngine::makeText(const TextBox& textBox)
{
	return nullptr;
}

TextEngine::~TextEngine()
{

}

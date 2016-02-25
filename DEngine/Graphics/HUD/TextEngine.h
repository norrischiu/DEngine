#pragma once
#include "TextBox.h"
#include "Graphics\MeshComponent.h"

class TextEngine
{
public:
	TextEngine();
	~TextEngine();

	MeshComponent* makeText(const TextBox& textBox);
	static TextEngine* getInstance();
	void destructAndCleanUp();

private:
	static TextEngine* m_instance;
};

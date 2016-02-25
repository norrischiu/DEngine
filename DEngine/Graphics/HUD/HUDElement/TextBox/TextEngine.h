#pragma once
#include <unordered_map>
#include "TextBox.h"
#include "Graphics/MeshComponent.h"

class TextEngine
{
public:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct TextMeshData
	{
		MeshData* meshData;
		float textWidth;
		float textHeight;
	};

	TextEngine();
	~TextEngine();

	MeshComponent* makeText(TextBox* textBox);
	MeshData* CreateTextMeshData(const char* sentence);
	float getTextWidth(const char* sentence);
	static TextEngine* getInstance();
	void removeCacheByID(const char* id);
	void destructAndCleanUp();

private:
	bool LoadFontData(char*);

	static TextEngine* m_instance;
	FontType* m_Font;
	std::unordered_map<const char*, MeshComponent*> m_cache;
};

#pragma once
#include "Graphics\VertexFormat.h"
#include "Graphics\MeshComponent.h"

namespace DE
{

struct FontType
{
	float left, right;
	int size;
};

class TextHelper
{
public:
	TextHelper();
	~TextHelper();

	MeshData* CreateTextMeshData(const char* sentence);

private:
	bool LoadFontData(char*);
	FontType* m_Font;
};

} // namespace DE


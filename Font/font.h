#pragma once
#include <d3d11.h>
#include "VertexFormat.h"
#include "D3D11Renderer.h"
#include "MeshComponent.h"

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


#include <iostream>
#include <iomanip>
#include <fstream>
#include "TextEngine.h"
#include "Graphics/VertexFormat.h"
#include "Graphics/D3D11Renderer.h"

namespace DE
{

TextEngine* TextEngine::m_instance;

TextEngine::TextEngine()
{
	if (!m_Font && !LoadFontData("../Assets/font_data.txt")) {
		OutputDebugStringW(L"Load font data error\n");
	}
}

float TextEngine::getTextWidth(const char* sentence)
{
	const int iNUmLetters = strlen(sentence);
	float textWidth = 0.0f;
	const float textSpacing = 0.05f;
	const float textHeight = 1.0f;

	for (int i = 0, letter = 0, index = 0; i < iNUmLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;
		textWidth += m_Font[letter].size / (16.0f / textHeight) + textSpacing;
	}

	textWidth -= textSpacing;

	return textWidth;
}

MeshData* TextEngine::CreateTextMeshData(const char* sentence) {
	// Get the number of letters in the sentence.
	const int iNumLetters = strlen(sentence);
	const int iNumVerts = iNumLetters * 6;
	const int iNumIndices = iNumVerts;

	const float textSpacing = 0.05f;
	const float textHeight = 1.0f;
	float textWidth = 0.0f;


	float drawX = 0.0f;
	float drawY = 0.0f;

	vertex1P1UV* pVertices = new vertex1P1UV[iNumVerts];
	unsigned int* pIndices = new unsigned int[iNumIndices];

	// Draw each letter onto a quad.
	for (int i = 0, letter = 0, index = 0; i < iNumLetters; i++)
	{
		if (sentence[i] == '\n')
		{
			drawY -= textHeight;
			drawX = 0.0f;
			continue;
		}

		letter = ((int)sentence[i]) - 32;
		textWidth = m_Font[letter].size / (16.0f / textHeight);

		// If the letter is a space then just move over three pixels.
		if (letter == 0) {
			drawX = drawX - textWidth;
		}
		else {
			// First triangle in quad
			// Top left
			pVertices[index].m_pos = Vector3(drawX, drawY, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].left;
			pVertices[index].m_UV[1] = 0.0f;
			index++;

			// Bottom right
			pVertices[index].m_pos = Vector3(drawX - textWidth, drawY - textHeight, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].right;
			pVertices[index].m_UV[1] = 1.0f;
			index++;

			// Bottom left
			pVertices[index].m_pos = Vector3(drawX, drawY - textHeight, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].left;
			pVertices[index].m_UV[1] = 1.0f;
			index++;

			// Second triangle in quad
			// Top left
			pVertices[index].m_pos = Vector3(drawX, drawY, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].left;
			pVertices[index].m_UV[1] = 0.0f;
			index++;

			// Top right
			pVertices[index].m_pos = Vector3(drawX - textWidth, drawY, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].right;
			pVertices[index].m_UV[1] = 0.0f;
			index++;

			// Bottom right
			pVertices[index].m_pos = Vector3(drawX - textWidth, drawY - textHeight, 0.0f);
			pVertices[index].m_UV[0] = m_Font[letter].right;
			pVertices[index].m_UV[1] = 1.0f;
			index++;

			// Update the x location for drawing by the size of the letter and one pixel
			drawX = drawX - (textWidth + textSpacing);
		}
	}

	for (int i = 0, j = 0; i < iNumIndices;) {
		pIndices[i++] = j++;
		pIndices[i++] = j++;
		pIndices[i++] = j++;
		pIndices[i++] = j++;
		pIndices[i++] = j++;
		pIndices[i++] = j++;
	}

	MeshData* meshData = new MeshData(pVertices, iNumVerts, pIndices, iNumIndices, sizeof(vertex1P1UV));
	RenderPass* textPass = new RenderPass;
	textPass->SetVertexShader("../DEngine/Shaders/VS_vertex1P1UV.hlsl");
	textPass->SetPixelShader("../DEngine/Shaders/PS_texture.hlsl");
	textPass->SetBlendState(State::ALPHA_BS);
	textPass->AddTexture(new Texture(Texture::SHADER_RESOURCES, 1, "../Assets/font.dds"));
	meshData->m_Material.AddPassToTechnique(textPass);

	return meshData;
}

bool TextEngine::LoadFontData(char* filename) {
	std::ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer.
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

TextEngine* TextEngine::getInstance()
{
	if (!m_instance) {
		m_instance = new TextEngine();
	}

	return m_instance;
}

MeshComponent* TextEngine::makeText(TextBox* textBox)
{
	const char* sentence = textBox->getText();
	char* id = textBox->getID();

	if (textBox->hasUpdate()) {
		m_cache.erase(id);
		textBox->setHasUpdate(false);
	}

	if (!m_cache[id]) {
		MeshData* meshData = CreateTextMeshData(sentence);
		RenderPass* pass = meshData->m_Material.GetRenderTechnique()->m_vRenderPasses[0];
		pass->SetRenderTargets(&D3D11Renderer::GetInstance()->m_backbuffer->GetRTV(), 1);
		pass->SetDepthStencilState(State::DISABLE_DEPTH_DISABLE_STENCIL_DSS);
		pass->SetRasterizerState(State::CULL_NONE_RS);
//		MeshComponent* meshComp = new MeshComponent(meshData);

		Matrix4 scaleX, scaleY, translate;
		scaleX.CreateScaleX(-1.0f * textBox->getFontSize());
		scaleY.CreateScaleY(1.0f * textBox->getFontSize());
		translate.CreateTranslation(Vector3(-1024 / 2.0f + textBox->getPosition().x, 768 / 2.0f - textBox->getPosition().y, 0.0f));
//		*meshComp->m_pTransform = Matrix4::OrthographicProjection(1024, 768, 0.0f, 1.0f) * translate * scaleX * scaleY;

//		m_cache[id] = meshComp;
	}

	return m_cache[id];
}

void TextEngine::removeCacheByID(const char* id)
{
	if (m_cache.find(id) != m_cache.end())
	{
		m_cache.erase(id);
	}
}

void TextEngine::destructAndCleanUp()
{
	if (m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

TextEngine::~TextEngine()
{

}

};
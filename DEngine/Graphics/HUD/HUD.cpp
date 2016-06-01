#include "HUD.h"
#include "HUDElement\TextBox\TextEngine.h"
#include "HUDElement\ProgressBar\ProgressBarEngine.h"
#include "Graphics\D3D11Renderer.h"
#include <windows.h>

namespace DE
{

HUD* HUD::m_instance;

HUD::HUD()
{
	m_pVSCBuffer = new VSPerObjectCBuffer;
}

TextBox* HUD::addText(char* id, char* string, const HUDElement::Position pos, const HUDElement::FontSize fontSizePt, const HUDElement::Color color, const float duration)
{
	if (m_elements.Contain(id)) {
		cleanUpCache(id);
	}

	TextBox* textBox = new TextBox(id, string, pos, fontSizePt, color, duration);

	m_elements[id] = textBox;
	m_timer[id] = 0.0f;

	return textBox;
}

ProgressBar* HUD::addProgress(char* id, const float progress, const HUDElement::Position pos, const HUDElement::Size size, const bool showText, const float duration) {
	ProgressBar* progressBar = new ProgressBar(id, progress, pos, size, showText, duration);
	m_elements[id] = progressBar;
	m_timer[id] = 0.0f;

	return progressBar;
}

HUDElement* HUD::getHUDElementById(const char* id)
{
	return m_elements[id];
}

void HUD::removeHUDElementById(const char* id)
{
	if (m_elements.Contain(id))
	{
		delete m_elements[id];
		m_elements.Remove(id);
	}

	if (m_timer.Contain(id))
	{
		m_timer.Remove(id);
	}

	cleanUpCache(id);
}

void HUD::cleanUpCache(const char* id)
{
	switch (m_elements[id]->getTypeID())
	{
	case HUDElement::TypeID::TEXTBOX:
		TextEngine::getInstance()->removeCacheByID(id);
		break;
	case HUDElement::TypeID::PROGRESSBAR:
		ProgressBarEngine::getInstance()->removeCacheByID(id);
		break;
	}
}

void HUD::update(const float delta_time)
{
	m_timer.ForEachPair([&](MyHashMap<float>::HashMapPair pair)
	{
		if ((int)m_elements[pair.m_Key]->getDuration() != -1) {
			pair.m_Item += delta_time;

			if (pair.m_Item > m_elements[pair.m_Key]->getDuration()) {
				cleanUpCache(pair.m_Key);
				m_elements.Remove(pair.m_Key);
				m_timer.Remove(pair.m_Key);
			}
		}
	});
}

void HUD::Render()
{
	m_pVSCBuffer->BindToRenderer();
	VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer->m_Memory._data;

	MeshComponent* meshComponent = nullptr;

	m_elements.ForEachItem([&](HUDElement* item)
	{
		if (item->isVisible())
		{
			switch (item->getTypeID()) {
			case HUDElement::TypeID::PROGRESSBAR:
			{
				meshComponent = ProgressBarEngine::getInstance()->makeProgress((ProgressBar*)item);
				int numMeshComponent = ((ProgressBar*)item)->isShowText() ? 2 : 1;
				for (int i = 0; i <= numMeshComponent; i++) {
					ptr->WVPTransform = *meshComponent[i].m_pTransform;
					m_pVSCBuffer->Update();
					meshComponent[i].m_pMeshData->Render();
				}
			}
			break;
			case HUDElement::TypeID::TEXTBOX:
				if (strcmp(((TextBox*)item)->getText(), "") != 0)
				{
					meshComponent = TextEngine::getInstance()->makeText((TextBox*)item);
					ptr->WVPTransform = *meshComponent->m_pTransform;
					m_pVSCBuffer->Update();

					meshComponent->m_pMeshData->Render();
				}
				break;
			}
		}
	});
}

void HUD::destructAndCleanUp()
{
	TextEngine::getInstance()->destructAndCleanUp();
	ProgressBarEngine::getInstance()->destructAndCleanUp();
}

HUD* HUD::getInstance()
{
	if (!m_instance) {
		m_instance = new HUD();
	}

	return m_instance;
}

HUD::~HUD()
{
}

};
#include "HUD.h"
#include "HUDElement\TextBox\TextEngine.h"
#include "HUDElement\ProgressBar\ProgressBarEngine.h"
#include "Graphics\D3D11Renderer.h"
#include <windows.h>

HUD* HUD::m_instance;

HUD::HUD()
{
	m_pVSCBuffer = new VSPerObjectCBuffer;
}

TextBox* HUD::addText(char* id, char* string, const HUDElement::Position pos, const HUDElement::FontSize fontSizePt, const HUDElement::Color color, const float duration)
{
	if (m_elements.find(id) != m_elements.end()) {
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
	if (m_elements.find(id) != m_elements.end())
	{
		m_elements.erase(id);
	}

	if (m_timer.find(id) != m_timer.end())
	{
		m_timer.erase(id);
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
	for (std::unordered_map<const char*, float>::iterator itr = m_timer.begin(); itr != m_timer.end();)
	{
		if ((int) m_elements[itr->first]->getDuration() != -1) {
			itr->second += delta_time;

			if (itr->second > m_elements[itr->first]->getDuration()) {
				cleanUpCache(itr->first);
				m_elements.erase(itr->first);
				itr = m_timer.erase(itr);
			} else {
				itr++;
			}
		} else {
			itr++;
		}
	}
}

void HUD::Render()
{
	m_pVSCBuffer->BindToRenderer();
	VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER* ptr = (VSPerObjectCBuffer::VS_PER_OBJECT_CBUFFER*) m_pVSCBuffer->m_Memory._data;

	MeshComponent* meshComponent = nullptr;

	for (auto l : m_elements)
	{
		if (l.second->isVisible())
		{
			switch (l.second->getTypeID()) {
				case HUDElement::TypeID::PROGRESSBAR:
				{
					meshComponent = ProgressBarEngine::getInstance()->makeProgress((ProgressBar*)l.second);
					int numMeshComponent = ((ProgressBar*)l.second)->isShowText() ? 2 : 1;
					for (int i = 0; i <= numMeshComponent; i++) {
						ptr->WVPTransform = *meshComponent[i].m_pTransform;
						m_pVSCBuffer->Update();
						meshComponent[i].m_pMeshData->Render();
					}
				}
					break;
				case HUDElement::TypeID::TEXTBOX:
					if (strcmp(((TextBox*)l.second)->getText(), "") != 0)
					{
						meshComponent = TextEngine::getInstance()->makeText((TextBox*)l.second);
						ptr->WVPTransform = *meshComponent->m_pTransform;
						m_pVSCBuffer->Update();

						meshComponent->m_pMeshData->Render();
					}
					break;
				}
		}
	}
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

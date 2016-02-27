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

void HUD::addText(char* id, char* string, const HUDElement::Position pos, const HUDElement::FontSize fontSizePt, const HUDElement::Color color, const float duration)
{
	m_elements[id] = new TextBox(id, string, pos, fontSizePt, color, duration);
	m_timer[id] = 0.0f;
}

void HUD::addProgress(char* id, const float progress, const HUDElement::Position pos, const HUDElement::Size size, const bool showText, const float duration) {
	m_elements[id] = new ProgressBar(id, progress, pos, size, showText, duration);
	m_timer[id] = 0.0f;
}

HUDElement* HUD::getHUDElementById(const char* id)
{
	return m_elements[id];
}

void HUD::removeHUDElementById(const char* id)
{
	m_elements.erase(id);
}

void HUD::update(const float delta_time)
{
	/*
	static wchar_t s[64];
	swprintf(s, 64, L"delta_time: %f\n", delta_time);
	OutputDebugStringW(s);
	*/

	static bool preventStartUpUpdate = true;
	if (preventStartUpUpdate && delta_time > 2.0f) {		//Hack
		preventStartUpUpdate = false;
		return;
	}

	for (std::unordered_map<const char*, float>::iterator itr = m_timer.begin(); itr != m_timer.end();)
	{
		if ((int) m_elements[itr->first]->getDuration() != -1) {
			itr->second += delta_time;

			if (itr->second > m_elements[itr->first]->getDuration()) {
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
				meshComponent = TextEngine::getInstance()->makeText((TextBox*) l.second);
				ptr->WVPTransform = *meshComponent->m_pTransform;
				m_pVSCBuffer->Update();

				meshComponent->m_pMeshData->Render();
				break;
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

#include "AudioSystem.h"
#include "../Event/AudioEvent.h"

namespace DE
{

AudioSystem* AudioSystem::m_pInstance = nullptr;

AudioSystem* AudioSystem::GetInstance()
{
	if (!m_pInstance) {
		m_pInstance = new AudioSystem();
	}

	return m_pInstance;
}

AudioSystem::AudioSystem()
{
	// This is only needed in Windows desktop apps
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;

#ifdef _DEBUG
	eflags = eflags | DirectX::AudioEngine_Debug;
#endif

	m_audEngine = std::unique_ptr<DirectX::AudioEngine>(new DirectX::AudioEngine(eflags));
}


AudioSystem::~AudioSystem()
{

}

AudioSystem::AudioState AudioSystem::GetAudioState(const int event_id, const char* audio_id)
{
	std::string hashKey = std::to_string(event_id) + '#' + std::string(audio_id);

	if (!m_soundEffectIns[hashKey])
	{
		return AudioSystem::STOPPED;
	}
	else
	{
		return (AudioSystem::AudioState) m_soundEffectIns[hashKey]->GetState();
	}
}

void AudioSystem::AddAudio(const char* id, const wchar_t* filename)
{
	std::wstring filepath = std::wstring(L"../Assets/") + filename + L".wav";

	m_soundEffect[std::string(id)] = std::unique_ptr<DirectX::SoundEffect>(new DirectX::SoundEffect(m_audEngine.get(), filepath.c_str()));
}

void AudioSystem::HandleAudioPlayEvent(Event* pEvt)
{
	if (pEvt->m_ID == AudioEventID::Audio_Play_Event)
	{
		Audio_Play_Event* pAudioPlayEvt = (Audio_Play_Event*)pEvt;

		std::string hashKey = std::to_string(pAudioPlayEvt->event_id) + '#' + pAudioPlayEvt->audio_id;

		m_soundEffectIns[hashKey] = m_soundEffect[pAudioPlayEvt->audio_id]->CreateInstance();
		m_soundEffectIns[hashKey]->SetVolume(pAudioPlayEvt->volume);
		m_soundEffectIns[hashKey]->SetPitch(pAudioPlayEvt->pitch);
		m_soundEffectIns[hashKey]->SetPan(pAudioPlayEvt->pan);
		m_soundEffectIns[hashKey]->Play(pAudioPlayEvt->looping);

		//Play will return false if too many sounds already are playing.
	}
}

void AudioSystem::HandleAudioStopEvent(Event* pEvt)
{

}

void AudioSystem::Update(float elapsedTime)
{
	//Per-frame processing
	if (!m_audEngine->Update())
	{
		// No audio device is active
		if (m_audEngine->IsCriticalError())
		{

		}
	}
	else
	{
		for (auto itr = m_soundEffectIns.begin(); itr != m_soundEffectIns.end();)
		{
			if (!itr->second || itr->second->GetState() == DirectX::SoundState::STOPPED)
			{
				itr = m_soundEffectIns.erase(itr);
			}
			else
			{
				itr++;
			}
		}

		while (!EventQueue::GetInstance()->Empty(AUDIO_EVENT))
		{
			Handle hEvt = EventQueue::GetInstance()->Front(AUDIO_EVENT);
			Event* pEvt = (Event*)hEvt.Raw();
			if (pEvt->m_ID == AudioEventID::Audio_Play_Event)
			{
				HandleAudioPlayEvent(pEvt);
			}
			if (pEvt->m_ID == AudioEventID::Audio_Stop_Event)
			{
				HandleAudioStopEvent(pEvt);
			}
			EventQueue::GetInstance()->Pop(AUDIO_EVENT);
		}
	}
}

};

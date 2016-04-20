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
	m_soundEffect.clear();
	m_soundEffectIns.clear();
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

void AudioSystem::AddAudio(const char* audio_id, const wchar_t* filename)
{
	std::wstring filepath = std::wstring(L"../Assets/") + filename + L".wav";

	m_soundEffect[std::string(audio_id)] = std::unique_ptr<DirectX::SoundEffect>(new DirectX::SoundEffect(m_audEngine.get(), filepath.c_str()));
}

bool AudioSystem::HasSoundEffect(const char* audio_id)
{
	if (m_soundEffect[std::string(audio_id)])
	{
		return true;
	}

	return false;
}

void AudioSystem::Play(const int event_id, const char* audio_id, const float volume, const float pitch, const float pan, const bool looping)
{
	std::string hashKey = std::to_string(event_id) + '#' + audio_id;

	if (!m_soundEffectIns[hashKey])
	{
		m_soundEffectIns[hashKey] = m_soundEffect[audio_id]->CreateInstance();
	}
	else
	{
		m_soundEffectIns[hashKey]->Stop();
	}

	m_soundEffectIns[hashKey]->SetVolume(volume);
	m_soundEffectIns[hashKey]->SetPitch(pitch);
	m_soundEffectIns[hashKey]->SetPan(pan);
	m_soundEffectIns[hashKey]->Play(looping);
}

void AudioSystem::Pause(const int event_id, const char* audio_id)
{
	std::string hashKey = std::to_string(event_id) + '#' + audio_id;

	if (m_soundEffectIns[hashKey])
	{
		m_soundEffectIns[hashKey]->Pause();
	}
}

void AudioSystem::Stop(const int event_id, const char* audio_id)
{
	std::string hashKey = std::to_string(event_id) + '#' + audio_id;

	if (m_soundEffectIns[hashKey])
	{
		m_soundEffectIns[hashKey]->Stop();
	}
}

void AudioSystem::HandleAudioStopEvent(Event* pEvt)
{
	if (pEvt->m_ID == AudioEventID::Audio_Stop_Event)
	{
		Audio_Stop_Event* pAudioPlayEvt = (Audio_Stop_Event*)pEvt;

		Pause(
			pAudioPlayEvt->event_id,
			pAudioPlayEvt->audio_id
		);
	}
}

void AudioSystem::HandleAudioPauseEvent(Event* pEvt)
{
	if (pEvt->m_ID == AudioEventID::Audio_Pause_Event)
	{
		Audio_Pause_Event* pAudioPlayEvt = (Audio_Pause_Event*)pEvt;

		Pause(
			pAudioPlayEvt->event_id,
			pAudioPlayEvt->audio_id
		);

		//Play will return false if too many sounds already are playing.
	}
}

void AudioSystem::HandleAudioPlayEvent(Event* pEvt)
{
	if (pEvt->m_ID == AudioEventID::Audio_Play_Event)
	{
		Audio_Play_Event* pAudioPlayEvt = (Audio_Play_Event*)pEvt;

		Play(
			pAudioPlayEvt->event_id,
			pAudioPlayEvt->audio_id,
			pAudioPlayEvt->volume,
			pAudioPlayEvt->pitch,
			pAudioPlayEvt->pan,
			pAudioPlayEvt->looping
		);

		//Play will return false if too many sounds already are playing.
	}
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
		/*
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
		*/

		while (!EventQueue::GetInstance()->Empty(AUDIO_EVENT))
		{
			Handle hEvt = EventQueue::GetInstance()->Front(AUDIO_EVENT);
			Event* pEvt = (Event*)hEvt.Raw();
			if (pEvt->m_ID == AudioEventID::Audio_Play_Event)
			{
				HandleAudioPlayEvent(pEvt);
			}
			else if (pEvt->m_ID == AudioEventID::Audio_Pause_Event)
			{
				HandleAudioPauseEvent(pEvt);
			}
			else if (pEvt->m_ID == AudioEventID::Audio_Stop_Event)
			{
				HandleAudioStopEvent(pEvt);
			}
			EventQueue::GetInstance()->Pop(AUDIO_EVENT);
		}
	}
}

};

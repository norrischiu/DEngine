#pragma once
#include "..\DirectXTK\Inc\Audio.h"
#include "..\Event/EventQueue.h"
#include <unordered_map>

namespace DE
{
	
class AudioSystem
{
public:
	enum AudioState
	{
		STOPPED = DirectX::SoundState::STOPPED,
		PLAYING = DirectX::SoundState::PLAYING,
		PAUSED = DirectX::SoundState::PAUSED
	};

	AudioSystem();
	~AudioSystem();

	static AudioSystem* GetInstance();

	void AddAudio(const char* id, const wchar_t* filename);
	AudioState GetAudioState(const int event_id, const char* audio_id);
	void HandleAudioPlayEvent(Event* pEvt);
	void HandleAudioStopEvent(Event* pEvt);
	void Update(float elapsedTime);

private:
	static AudioSystem* m_pInstance;
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffect>> m_soundEffect;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffectInstance>> m_soundEffectIns;
};

};
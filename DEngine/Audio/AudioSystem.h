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

	void Init();
	void Reset();

	static AudioSystem* GetInstance();

	void AddAudio(const char* audio_id, const wchar_t* filename);
	void RemoveAudio(const char* audio_id);
	bool HasSoundEffect(const char* audio_id);
	AudioState GetAudioState(const int event_id, const char* audio_id);

	void Play(const int event_id, const char* audio_id, const float volume, const float pitch, const float pan, const bool looping);
	void Pause(const int event_id, const char* audio_id);
	void Stop(const int event_id, const char* audio_id);

	virtual void HandleAudioPlayEvent(Event* pEvt);
	virtual void HandleAudioPauseEvent(Event* pEvt);
	virtual void HandleAudioStopEvent(Event* pEvt);
	virtual void Update(float elapsedTime);

private:
	static AudioSystem* m_pInstance;
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffect>> m_soundEffect;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffectInstance>> m_soundEffectIns;
};

};
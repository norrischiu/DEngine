#pragma once
#include "..\DirectXTK\Inc\Audio.h"

namespace DE
{
	
class GameAudio
{
public:
	static int SOUND_EFFECT_ID;

	GameAudio();
	~GameAudio();

	static GameAudio* GetInstance();

	int AddAudio(const wchar_t* filename);
	void Play(const int id, const bool looping = true);
	void Update(float elapsedTime);

private:
	static GameAudio* m_pInstance;
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	std::vector<std::unique_ptr<DirectX::SoundEffect>> m_soundEffect;
};

};
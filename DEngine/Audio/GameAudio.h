#pragma once
#include "..\DirectXTK\Inc\Audio.h"

namespace DE
{
	
class GameAudio
{
public:
	GameAudio();
	~GameAudio();

	static GameAudio* GetInstance();

	void Init(const wchar_t* filename);
	void Play(const bool looping = false);
	void Update(float elapsedTime);

private:
	static GameAudio* m_pInstance;
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	std::unique_ptr<DirectX::SoundEffect> m_soundEffect;
};

};
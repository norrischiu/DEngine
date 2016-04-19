#include "GameAudio.h"

namespace DE
{

GameAudio* GameAudio::m_pInstance = nullptr;
int GameAudio::SOUND_EFFECT_ID = 0;

GameAudio* GameAudio::GetInstance()
{
	if (!m_pInstance) {
		m_pInstance = new GameAudio();
	}

	return m_pInstance;
}

GameAudio::GameAudio()
{

}


GameAudio::~GameAudio()
{
	// This is only needed in Windows desktop apps
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;

	#ifdef _DEBUG
		eflags = eflags | DirectX::AudioEngine_Debug;
	#endif

	m_audEngine = std::unique_ptr<DirectX::AudioEngine>(new DirectX::AudioEngine(eflags));
}

int GameAudio::AddAudio(const wchar_t* filename)
{
	m_soundEffect[SOUND_EFFECT_ID] = std::unique_ptr<DirectX::SoundEffect>(new DirectX::SoundEffect(m_audEngine.get(), filename));

	return SOUND_EFFECT_ID++;
}

void GameAudio::Play(const int id, const bool looping)
{
	if (looping)
	{
		m_soundEffect[id]->CreateInstance()->Play(true);
	}
	else
	{
		m_soundEffect[id]->Play();
	}
}

void GameAudio::Update(float elapsedTime)
{
	//Per-frame processing
	if (!m_audEngine->Update())
	{
		// No audio device is active
		if (m_audEngine->IsCriticalError())
		{
	
		}
	}
}

};

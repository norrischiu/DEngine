#include "GameAudio.h"

namespace DE
{

GameAudio* GameAudio::m_pInstance = nullptr;

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
}

void GameAudio::Init(const wchar_t* filename)
{
	// This is only needed in Windows desktop apps
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;

#ifdef _DEBUG
	eflags = eflags | DirectX::AudioEngine_Debug;
#endif

	m_audEngine = std::unique_ptr<DirectX::AudioEngine>(new DirectX::AudioEngine(eflags));
	m_soundEffect = std::unique_ptr<DirectX::SoundEffect>(new DirectX::SoundEffect(m_audEngine.get(), filename));
}

void GameAudio::Play(const bool looping)
{
	if (looping)
	{
		m_soundEffect->CreateInstance()->Play(true);
	}
	else
	{
		m_soundEffect->Play();
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

#include "AudioComponent.h"
#include "AudioSystem.h"
#include "../Event/AudioEvent.h"
#include "../Event/EventQueue.h"
#include "../Graphics/D3D11Renderer.h"

// C++ include
#include <sstream>

namespace DE
{

AudioComponent::AudioComponent(AnimationStateMachine* ASM) : m_ASM(ASM)
{
	m_ID = ComponentID;
}


AudioComponent::~AudioComponent()
{
}

void AudioComponent::Update(float deltaTime)
{
	const Vector3 camPos = D3D11Renderer::GetInstance()->GetCamera()->GetPosition();
	const Vector3 ownerPos = GetOwner()->GetPosition();

	//distance between game object and render camera position
	const float distance = (camPos - ownerPos).Length();
	const float DISTANCE_THRESHOLD = 10.0f;

	if (distance < DISTANCE_THRESHOLD)
	{
		AnimationStateMachine::State* currState = m_ASM->GetCurrentState();

		if (AudioSystem::GetInstance()->GetAudioState(GetOwner()->GetGameObjectID(), currState->m_sClipName) != AudioSystem::PLAYING)
		{
			DE::Handle hEvt(sizeof(Audio_Play_Event));
			Audio_Play_Event* evt = new (hEvt) Audio_Play_Event
			(
				GetOwner()->GetGameObjectID(),							//event id
				currState->m_sClipName,									//audio id		
				min((1.0f / (distance + 1.0f)), 1.0f),					//volumn
				0.0f,													//pitch
				0.0f,													//pan
				false													//looping
			);

			EventQueue::GetInstance()->Add(hEvt, AUDIO_EVENT);
		}
	}
}

}
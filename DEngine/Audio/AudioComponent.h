#pragma once
#include "Object\Component.h"
#include "../Graphics/Animation/AnimationStateMachine.h"

namespace DE
{

class AudioComponent : public Component
{
public:
	static const int ComponentID = ComponentID::AUDIO_COMPONENT;

	AudioComponent(AnimationStateMachine* ASM);
	~AudioComponent();

	void Update(float deltaTime) override;

private:
	static AnimationStateMachine* m_ASM;
};

}

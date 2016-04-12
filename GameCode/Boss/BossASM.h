#pragma once

#ifndef BOSS_ASM_H_
#define BOSS_ASM_H_

// Engine include
#include "DEngine\Graphics\Animation\AnimationStateMachine.h"
#include "DEngine\Memory\Handle.h"

// Game include
#include "Boss.h"

class BossASM : public DE::AnimationStateMachine
{

public:

	BossASM(DE::AnimationController* animController);

	// Inherited via AnimationStateMachine
	void Update(float deltaTime) override;

	// Inherited via AnimationStateMachine
	bool HandleEvent(DE::Handle hEvt) override;

	float time;
};

#endif // !BOSS_ASM_H_

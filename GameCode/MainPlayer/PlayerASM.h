#ifndef PLAYER_ASM_H_
#define PLAYER_ASM_H_

// Engine include
#include "DEngine\Graphics\Animation\AnimationStateMachine.h"
#include "DEngine\Memory\Handle.h"

class PlayerASM : public DE::AnimationStateMachine
{

public:

	PlayerASM(DE::AnimationController* animController);

	// Inherited via AnimationStateMachine
	void Update(float deltaTime) override;

	// Inherited via AnimationStateMachine
	void HandleEvent(DE::Handle hEvt) override;
};

#endif // !PLAYER_ASM_H_

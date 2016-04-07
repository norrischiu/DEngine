#ifndef PLAYER_MC_H_
#define PLAYER_MC_H_

// Engine include
#include "DEngine\Object\MovementController.h"

class PlayerMC : public DE::MovementController
{

public:

	PlayerMC()
		: DE::MovementController()
	{};

private:

	void HandleKeyboardEvent(DE::Event* pEvt) override;
	void HandleMouseEvent(DE::Event* pEvt) override;

	void Dispatch() override;

};

#endif // !PLAYER_ASM_H_

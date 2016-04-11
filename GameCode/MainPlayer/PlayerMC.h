#ifndef PLAYER_MC_H_
#define PLAYER_MC_H_

// Engine include
#include "DEngine\Object\MovementController.h"

class PlayerMC : public DE::MovementController
{

public:

	PlayerMC()
		: DE::MovementController()
		, m_bDodge(false)
		, m_fComboTime(0.0f)
	{
		m_ComboSequence[0] = false;
		m_ComboSequence[1] = false;
		m_ComboSequence[2] = false;
		m_ComboDelays[0] = 0.6f;
		m_ComboDelays[1] = 0.7f;
	};

	bool								m_ComboSequence[3];
	float								m_ComboDelays[2];
	float								m_fComboTime;

private:

	bool HandleKeyboardEvent(DE::Event* pEvt) override;
	bool HandleMouseEvent(DE::Event* pEvt) override;

	void Dispatch() override;

	bool								m_bDodge;

	DE::Vector3							m_vDodgeDir;

};

#endif // !PLAYER_ASM_H_

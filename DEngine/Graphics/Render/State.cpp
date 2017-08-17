#include "State.h"

#ifdef D3D12
	State::D3D12_STATE_DESC State::m_States[20];
#elif defined D3D11
	void* State::m_States[20];
#endif

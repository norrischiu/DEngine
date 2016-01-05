#ifndef WIN_MSG_HANDLER_H_
#define WIN_MSG_HANDLER_H_

#include <Windowsx.h>
#include "System/Keyboard.h"
#include "System/Mouse.h"

struct WinMsgHandler
{
	static void Handle(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// translate keyboard/mouse message to engine specific setting
		//if (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
		if (msg == WM_MOUSEMOVE)
		{
			Mouse::SetCursorPos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		//else if (msg >= WM_KEYFIRST && msg <= WM_KEYLAST)
		else if (msg == WM_KEYDOWN)
		{
			switch (wParam)
			{
				case VK_W:
				case VK_S:
				case VK_A:
				case VK_D:
					Keyboard::SetInputKey(wParam, true);
			}
		}
		else if (msg == WM_KEYUP)
		{
			switch (wParam)
			{
				case VK_W:
				case VK_S:
				case VK_A:
				case VK_D:
					Keyboard::SetInputKey(wParam, false);
			}
		}
		/*
			else if (wParam == 's')
			{
			}
		}
		*/

	}
};

#endif 

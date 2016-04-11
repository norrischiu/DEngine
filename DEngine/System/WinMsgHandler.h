#ifndef WIN_MSG_HANDLER_H_
#define WIN_MSG_HANDLER_H_

#include <Windowsx.h>
#include "System\Keyboard.h"
#include "System\Mouse.h"

using namespace DE;

struct WinMsgHandler
{
	static void Handle(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// translate keyboard/mouse message to engine specific setting
		//if (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
		if (msg == WM_MOUSEMOVE)
		{
			Mouse::SetMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		else if (msg == WM_LBUTTONDOWN)
		{
			Mouse::SetButton(MK_LBUTTON, true);
		}
		else if (msg == WM_LBUTTONUP)
		{
			Mouse::SetButton(MK_LBUTTON, false);
		}
		else if (msg == WM_RBUTTONDOWN)
		{
			Mouse::SetButton(MK_RBUTTON, true);
		}
		else if (msg == WM_RBUTTONUP)
		{
			Mouse::SetButton(MK_RBUTTON, false);
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
	}
};

#endif 

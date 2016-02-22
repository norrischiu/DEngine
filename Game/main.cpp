#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Timer\Timer.h"
#include "Graphics\D3D11Renderer.h"
#include <sstream>
#include "Memory\MemoryManager.h"
#include "System\WinMsgHandler.h"
#include "System\Keyboard.h"
#include "GameLoop.h"

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if (msg == WM_DESTROY) 
	{
		PostQuitMessage(0);
		return 0;
	}
	else if (msg == WM_PAINT) 
	{
		ValidateRect(hWnd, NULL);
		return 0;
	}
	else
	{
		InvalidateRect(hWnd, NULL, TRUE);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// Register the window class, in unicode
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Game class", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow("Game class", "Game Engine",
		WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768,
		NULL, NULL, wc.hInstance, NULL);

	// Memory
	MemoryManager::GetInstance()->ConstructDefaultPool();

	// Setup GraphicsDevice singletons
	D3D11Renderer::GetInstance()->ConstructWithWindow(hWnd);

	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	/// Timer
	Timer m_Timer;
	const float FPS = 30.0f;
	float elaspedTime = 0.0f;

	// Main loop
	bool bQuit = false;
	while (!bQuit)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				bQuit = true;
				break;
			}
			WinMsgHandler::Handle(msg.message, msg.wParam, msg.lParam);
		}

		// Inside one frame
		if (elaspedTime >= 1.0 / FPS)
		{

			// send input event by checking state change
			Keyboard::Update(elaspedTime);
			Mouse::Update(elaspedTime);

			// Update the game world based on delta time
			GameLoop::GetInstance()->Update(elaspedTime);
			D3D11Renderer::GetInstance()->Update();

			// Render this frame
			D3D11Renderer::GetInstance()->Render();

			// Debug text
			std::stringstream str;
			str << "FPS: " << 1.0 / elaspedTime;
			SetWindowText(hWnd, str.str().c_str());
			elaspedTime = 0.0f;
		}


		m_Timer.tick();
		elaspedTime += m_Timer.getDeltaTime();
	}

	// Cleanup the GameWorld and GraphicsDevice singletons
	D3D11Renderer::GetInstance()->DestructandCleanUp();
	//MemoryManager::GetInstance()->Destruct();

	UnregisterClass("Game class", wc.hInstance);
	return 0;
}
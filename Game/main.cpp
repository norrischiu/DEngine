// main.cpp: entry point

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "..\Graphics\D3D11Renderer.h"

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	// send the quit message when the window is destroyed
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;
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

	// Register the window class
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

	// Setup our GameWorld and GraphicsDevice singletons
	D3D11Renderer::getInstance()->ConstructWithWindow(hWnd);

	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// enter the main game loop
	bool bQuit = false;
	while (!bQuit)
	{
		// Update the game world based on delta time
		D3D11Renderer::getInstance()->Update();

		// Render this frame
		D3D11Renderer::getInstance()->Render();

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
		}
	}

	// Cleanup the GameWorld and GraphicsDevice singletons
	D3D11Renderer::getInstance()->DestructandCleanUp();

	UnregisterClass("Game class", wc.hInstance);
	return 0;
}

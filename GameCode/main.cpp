#define WIN32_LEAN_AND_MEAN

// Engine include
#include "DEngine\Timer\Timer.h"
#include "DEngine\Memory\MemoryManager.h"
#include "DEngine\System\WinMsgHandler.h"
#include "DEngine\System\Keyboard.h"
#include "DEngine\System\Mouse.h"
#include "DEngine\GlobalInclude.h"
#include "DEngine\Graphics\D3DRenderer.h"
#include "DEngine\Graphics\D3D11Renderer.h"
#include "DEngine\Graphics\D3D12Renderer.h"
#include "GameLoop.h"
#include <windows.h>

// C++ include
#include <sstream>

#ifdef D3D12
	DE::D3DRenderer* DE::D3DRenderer::m_pInstance = new DE::D3D12Renderer();
#elif defined D3D11
	DE::D3DRenderer* DE::D3DRenderer::m_pInstance = new DE::D3D11Renderer();
#endif

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
	HWND hWnd = CreateWindow("Game class", "DEngine",
		WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL, wc.hInstance, NULL);

	// Memory
	MemoryManager::GetInstance()->ConstructDefaultPool();

	// Setup GraphicsDevice singletons
	((D3D12Renderer*)D3DRenderer::GetInstance())->ConstructWithWindow(hWnd);

	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// Hide mouse
	ShowCursor(false);

	GameLoop::GetInstance()->Construct();		//Load the game content first before entering to the main loop

	/// Timer
	Timer m_Timer;
	const float FPS = 60.0f;
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
		if (elaspedTime >= 1.0f / FPS)
		{

			// send input event by checking state change
			DE::Keyboard::Update(elaspedTime);
			DE::Mouse::Update(elaspedTime);

			// Update the game world based on delta time
			GameLoop::GetInstance()->Update(elaspedTime);
			((D3D12Renderer*)D3DRenderer::GetInstance())->Update(elaspedTime);

			// Render this frame
			((D3D12Renderer*)D3DRenderer::GetInstance())->Render();

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
	((D3D12Renderer*)D3DRenderer::GetInstance())->DestructandCleanUp();
	MemoryManager::GetInstance()->Destruct();

	UnregisterClass("Game class", wc.hInstance);
	return 0;
}
#include "Main.h"
#include "Common.h"

int APIENTRY WinMain(	// Main entrypoint for the application
	_In_     HINSTANCE p_Instance,
	_In_opt_ HINSTANCE p_PreviousInstance,
	_In_     LPSTR p_CommandLine,
	_In_     int p_ShowCommand)
{

	if (IsGameAlreadyRunning())
		return ERROR_GAME_ALREADY_RUNNING;
	
	if (CreateMainGameWindow() == FAILED)
		return CRGetLastError();
	
	if (InitializeGame(g_hwnd) == FAILED)
		return CRGetLastError();

	MSG message = { 0 };
	g_Running = TRUE;

	while (g_Running)	// Message / Window loop
	{
		while (PeekMessage(&message, g_hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		
		// process player input
		ProcessPlayerInput();

		// update game

		// render game
		RenderFrameGraphics();

		Sleep(1);
	}

	TerminateGame();
	return 0; 
}

LRESULT CALLBACK MainWindowProcedure(
	HWND	p_Hwnd, 
	UINT	p_Msg, 
	WPARAM	p_Wide, 
	LPARAM	p_Low )
{
	switch (p_Msg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			g_Running = FALSE;
		}
	default:
		return DefWindowProc(p_Hwnd, p_Msg, p_Wide, p_Low);
	}
}

int CreateMainGameWindow()
{
	WNDCLASS windowClass; // initialize window class
	ZeroMemory(&windowClass, sizeof(windowClass));
	{
		windowClass.lpfnWndProc = MainWindowProcedure;
		windowClass.style = 0;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetInstance();
		windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = CreateSolidBrush(RGB(255, 0, 255));
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = GAME_NAME;
	}

	if (!(RegisterClass(&windowClass))) // register the window class
	{
		CRSetLastError(ERROR_WINDOW_REGISTRATION);
		MessageBox(NULL, L"Window registration failed!", L"Error", MB_OK | MB_ICONERROR);
		return FAILED;
	}

	g_hwnd = CreateWindow(					// Creating the window
		windowClass.lpszClassName,
		windowClass.lpszClassName,			// Window name
		WS_OVERLAPPEDWINDOW,				// Styles 
		CW_USEDEFAULT, CW_USEDEFAULT,		// Pos x, y
		768, 432,							// Width, Height
		NULL, NULL,
		GetInstance(),
		NULL
	);

	if (!g_hwnd) // return if the window creation wasn't successful
	{
		CRSetLastError(ERROR_WINDOW_CREATION);
		MessageBox(NULL, L"Window creation failed!", L"Error", MB_OK | MB_ICONERROR);
		return FAILED;
	}
	ShowWindow(g_hwnd, SW_SHOW);

	return 0;
}

BOOL IsGameAlreadyRunning(void)
{
	HANDLE mutex = { 0 };
	mutex = CreateMutex(NULL, GAME_NAME, L"_GameMutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return 1; // true
	}
	return 0; // false
}
